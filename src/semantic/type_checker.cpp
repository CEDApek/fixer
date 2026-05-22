#include "type_checker.hpp"

#include "common.hpp"

TypeChecker::TypeChecker() {
  // 你需要在这里对 symbol_table 进行初始化
  // 插入一些内置函数，如 read 和 write
  symbol_table.enter_scope();  // global scope

  // int read()
  symbol_table.add_symbol(
      "read",
      FuncType::create(PrimitiveType::Int, {})
  );

  // void write(int)
  symbol_table.add_symbol(
      "write",
      FuncType::create(PrimitiveType::Void, {PrimitiveType::Int})
  );
}

TypePtr TypeChecker::check(AST::NodePtr node) {
#define CHECK_NODE(type)                                     \
  if (auto n = std::dynamic_pointer_cast<AST::type>(node)) { \
    return check##type(n);                                   \
  }

  // 递归检查 AST 的每个节点
  // 如果你添加了新的 AST 节点类型，记得在这里添加对应的检查函数
  CHECK_NODE(CompUnit)
  CHECK_NODE(FuncDef)
  CHECK_NODE(VarDecl)
  CHECK_NODE(Block)
  CHECK_NODE(AssignStmt)
  CHECK_NODE(ReturnStmt)
  CHECK_NODE(LVal)
  CHECK_NODE(IntConst)
  CHECK_NODE(FuncCall)
  CHECK_NODE(UnaryExp)
  CHECK_NODE(BinaryExp)
  CHECK_NODE(IfStmt)
  CHECK_NODE(WhileStmt)
  CHECK_NODE(InitVal)
  CHECK_NODE(ExprStmt)

#undef CHECK_NODE

  ASSERT(false, "Unknown AST node type " + node->to_string() +
                    " in type checking at line " +
                    std::to_string(node->lineno));
}

TypePtr TypeChecker::checkCompUnit(AST::CompUnitPtr node) {
  for (auto& unit : node->units) {
    check(unit);
  }
  return nullptr;
}

TypePtr TypeChecker::checkExprStmt(AST::ExprStmtPtr node) {
  if (node->exp) check(node->exp);
  return nullptr;
}

TypePtr TypeChecker::checkInitVal(AST::InitValPtr node) {
  // scalar init: just forward to the wrapped expression
  if (node->expr) {
    return check(node->expr);
  }

  for (auto& elem : node->elements) {
    check(elem);
  }

  return nullptr;
}

bool TypeChecker::is_list_init(AST::InitValPtr node) {
  return node && !node->elements.empty();
}

int TypeChecker::count_init_elements(AST::InitValPtr node) {
  if (!node) return 0;

  // scalar form: InitVal <expr>
  if (node->expr) return 1;

  int total = 0;
  for (auto& elem : node->elements) {
    auto child = std::dynamic_pointer_cast<AST::InitVal>(elem);
    if (child) {
      total += count_init_elements(child);
    } else {
      total += 1;
    }
  }
  return total;
}

TypePtr TypeChecker::checkFuncDef(AST::FuncDefPtr node) {
  // 在这个函数中，你需要判断函数是否已经被定义过
  // 如果函数已经被定义过，你需要报错
  // 否则，你需要将函数插入符号表，并在符号表中创建一个新的作用域
  // 再将函数参数也插入符号表，并将符号表中对应的 symbol 挂到 FuncDef 节点上
  // 最后检查函数体的语句块
  TypePtr ret_type = PrimitiveType::create(node->return_btype);

  std::vector<TypePtr> param_types;
  for (auto& param : node->params) {
    TypePtr ptype = PrimitiveType::create(param->btype);

    if (param->is_array) {
      std::vector<int> dims;
      dims.push_back(-1);  // unspecified first dimension for array parameter
      for (int d : param->dims_after_first) dims.push_back(d);
      ptype = ArrayType::create(ptype, dims);
    }

    param_types.push_back(ptype);
  }

  TypePtr func_type = FuncType::create(ret_type, param_types);

  auto existing = symbol_table.find_symbol(node->name, true);
  if (existing) {
    std::cerr << "Semantic Error at line " << node->lineno
              << ": redefinition of function '" << node->name << "'\n";
    std::exit(11);
  }

  node->symbol = symbol_table.add_symbol(node->name, func_type);

  auto old_return_type = current_return_type;
  current_return_type = ret_type;

  symbol_table.enter_scope();

  for (size_t i = 0; i < node->params.size(); ++i) {
    auto& param = node->params[i];
    auto psym = symbol_table.add_symbol(param->ident, param_types[i]);
    //param->symbol = psym;
    if (!psym) {
      std::cerr << "Semantic Error at line " << param->lineno
                << ": redefinition of parameter '" << param->ident << "'\n";
      std::exit(11);
    }
    // only if Param has symbol field
    param->symbol = psym;
  }

  checkBlock(node->block, false);

  symbol_table.exit_scope();
  current_return_type = old_return_type;
  return nullptr;
}

TypePtr TypeChecker::checkVarDecl(AST::VarDeclPtr node) {
  for (auto var_def : node->defs) {
    checkVarDef(var_def, node->btype);
  }
  return nullptr;
}

int TypeChecker::array_capacity(const std::vector<int>& dims) {
  int total = 1;
  for (int d : dims) total *= d;
  return total;
}

int TypeChecker::validate_array_init(AST::InitValPtr init,
                                     const std::vector<int>& dims) {
  // dims describes an array type; init must be a list here
  int total = array_capacity(dims);
  int used = 0;

  for (auto& elem : init->elements) {
    auto child = std::dynamic_pointer_cast<AST::InitVal>(elem);

    if (child && child->expr) {
      // scalar element
      TypePtr t = check(child->expr);
      if (!t->compatible(PrimitiveType::Int)) {
        std::exit(4);
      }
      used += 1;
    } else if (child) {
      // nested list: must align to a proper subarray (not the whole current one)
      int chosen_j = -1;

      // choose the largest possible subarray => smallest j
      for (size_t j = 1; j < dims.size(); ++j) {
        std::vector<int> sub_dims(dims.begin() + j, dims.end());
        int sub_cap = array_capacity(sub_dims);
        if (used % sub_cap == 0) {
          chosen_j = (int)j;
          break;
        }
      }

      if (chosen_j == -1) {
        // list appears where only scalar is allowed
        std::exit(3);
      }

      std::vector<int> sub_dims(dims.begin() + chosen_j, dims.end());
      validate_array_init(child, sub_dims);
      used += array_capacity(sub_dims);  // brace fills the whole chosen subarray
    } else {
      // fallback, in case parser stored a raw expr directly
      TypePtr t = check(elem);
      if (!t->compatible(PrimitiveType::Int)) {
        std::exit(4);
      }
      used += 1;
    }

    if (used > total) {
      std::exit(3);
    }
  }

  return used;
}

TypePtr TypeChecker::checkVarDef(AST::VarDefPtr node, BasicType var_type) {
  // 你需要判断变量是否已经被定义过，并更新符号表
  //auto type = PrimitiveType::create(var_type);
  // 判断变量是否已经被定义过
  // 如果有初始化表达式，你需要检查初始化表达式的类型是否和变量类型相同
  // 如果是数组，你还需要检查初始化表达式和数组的维度是否匹配，是否有溢出的情况
  TypePtr type = PrimitiveType::create(var_type);

  if (!node->dims.empty()) {
    type = ArrayType::create(type, node->dims);
  }

  auto existing = symbol_table.find_symbol(node->ident, true);
  if (existing) {
    std::cerr << "Semantic Error at line " << node->lineno
              << ": redefinition of identifier '" << node->ident << "'\n";
    std::exit(11);
  }

if (node->init) {
  if (!node->dims.empty()) {
    auto init_val = std::dynamic_pointer_cast<AST::InitVal>(node->init);
    if (!init_val) {
      std::exit(12);
    }

    // array initializer must be a list, not scalar expr
    if (init_val->expr) {
      std::exit(12);
    }

    validate_array_init(init_val, node->dims);
  } else {
    auto init_val = std::dynamic_pointer_cast<AST::InitVal>(node->init);

    // scalar variable initialized by list
    if (init_val && !init_val->expr) {
      std::exit(3);
    }

    TypePtr init_type = check(node->init);
    if (!type->compatible(init_type)) {
      std::exit(4);
    }
  }
}

  node->symbol = symbol_table.add_symbol(node->ident, type);
  return nullptr;
}

TypePtr TypeChecker::checkBlock(AST::BlockPtr node, bool new_scope) {
  // 检查块内的每个语句
  // 如果 new_scope 为 true
  // 你需要在进入和退出块时更新符号表，创建、销毁新的作用域
  if (new_scope) {
    symbol_table.enter_scope();
  }

  for (auto& item : node->stmts) {
    check(item);
  }

  if (new_scope) {
    symbol_table.exit_scope();
  }

  return nullptr;
}

TypePtr TypeChecker::checkAssignStmt(AST::AssignStmtPtr node) {
  TypePtr lval_type = check(node->lval);
  TypePtr expr_type = check(node->exp);
  // 判断赋值号两边的类型是否相同
  // 我们实验中只支持 int 类型
  // 因此你需要判断 lval_type 和 expr_type 是否都为 int 类型
  if (std::dynamic_pointer_cast<ArrayType>(lval_type)) {
    std::exit(2);
  }

  if (!lval_type->compatible(expr_type)) {
    std::exit(4);
  }

  return lval_type;
}

TypePtr TypeChecker::checkReturnStmt(AST::ReturnStmtPtr node) {
  // 判断返回值类型是否和函数声明的返回值类型相同
  if (!node->exp) {
    if (!current_return_type->compatible(PrimitiveType::Void)) {
      std::cerr << "Semantic Error at line " << node->lineno
                << ": return type mismatch\n";
      std::exit(7);
    }
    return nullptr;
  }

  TypePtr ret_type = check(node->exp);
  if (!current_return_type->compatible(ret_type)) {
    std::cerr << "Semantic Error at line " << node->lineno
              << ": return type mismatch\n";
    std::exit(7);
  }

  return nullptr;
}

TypePtr TypeChecker::checkIfStmt(AST::IfStmtPtr node) {
  TypePtr cond_type = check(node->cond);

  if (!cond_type->compatible(PrimitiveType::Int)) {
    std::exit(4);
  }

  check(node->then_stmt);
  if (node->else_stmt) check(node->else_stmt);

  return nullptr;
}

TypePtr TypeChecker::checkWhileStmt(AST::WhileStmtPtr node) {
  TypePtr cond_type = check(node->cond);

  if (!cond_type->compatible(PrimitiveType::Int)) {
    std::exit(4);
  }

  check(node->body);
  return nullptr;
}

TypePtr TypeChecker::checkLVal(AST::LValPtr node) {
  // 你需要在这里查找符号表，判断变量是否被定义过
  // 根据符号表中的信息设置 LVal 的类型
  // 若变量未定义，你需要报错
  // 否则，将符号表中的 symbol 挂到 LVal 节点上
  // 如果 LVal 是数组，你还需要根据下标索引来设置 LVal 的类型
  auto symbol = symbol_table.find_symbol(node->ident);
  if (!symbol) {
    std::cerr << "Semantic Error at line " << node->lineno
              << ": use of undeclared identifier '" << node->ident << "'\n";
    std::exit(10);
  }

  node->symbol = symbol;
  TypePtr type = symbol->type;

  for (auto& idx : node->indices) {
    auto arr_type = std::dynamic_pointer_cast<ArrayType>(type);
    if (!arr_type) {
      std::cerr << "Semantic Error at line " << node->lineno
                << ": subscripted value is not an array\n";
      std::exit(6);
    }

    TypePtr idx_type = check(idx);
    if (!idx_type->compatible(PrimitiveType::Int)) {
      std::cerr << "Semantic Error at line " << node->lineno
                << ": array subscript is not an integer\n";
      std::exit(8);
    }

    if (arr_type->dims.empty()) {
      std::exit(6); // too many subscripts
    }

    if (arr_type->dims.size() == 1) {
      type = arr_type->element_type;
    } else {
      std::vector<int> rest(arr_type->dims.begin() + 1, arr_type->dims.end());
      type = ArrayType::create(arr_type->element_type, rest);
    }
  }

  return type;
}

TypePtr TypeChecker::checkIntConst(AST::IntConstPtr node) {
  // 整数常量的类型是 int
  return PrimitiveType::Int;
}

TypePtr TypeChecker::checkFuncCall(AST::FuncCallPtr node) {
  // 首先需要查找函数是否被定义过
  // 然后需要判断函数调用的参数个数和类型是否和声明一致
  // 最后设置函数调用表达式的类型为函数的返回值类型
  // 并将函数的 symbol 挂到 FuncCall 节点上
  auto symbol = symbol_table.find_symbol(node->name);
  if (!symbol) {
    std::cerr << "Semantic Error at line " << node->lineno
              << ": use of undeclared function '" << node->name << "'\n";
    std::exit(10);
  }

  auto func_type = std::dynamic_pointer_cast<FuncType>(symbol->type);
  if (!func_type) {
    std::cerr << "Semantic Error at line " << node->lineno
              << ": called object is not a function\n";
    std::exit(9);
  }

  if (node->args.size() != func_type->param_types.size()) {
    std::cerr << "Semantic Error at line " << node->lineno
              << ": function arguments number not matched\n";
    std::exit(13);
  }

  for (size_t i = 0; i < node->args.size(); ++i) {
    TypePtr actual = check(node->args[i]);
    TypePtr formal = func_type->param_types[i];

    auto formal_arr = std::dynamic_pointer_cast<ArrayType>(formal);
    auto actual_arr = std::dynamic_pointer_cast<ArrayType>(actual);

    if (formal_arr && actual_arr) {
      if (!formal_arr->element_type->compatible(actual_arr->element_type)) {
        std::exit(4);
      }

      // formal array param has leading -1 placeholder
      if (formal_arr->dims.empty() || formal_arr->dims[0] != -1) {
        if (!formal_arr->compatible(actual_arr)) {
          std::exit(4);
        }
      } else {
        if (actual_arr->dims.size() != formal_arr->dims.size()) {
          std::exit(4);
        }

        for (size_t j = 1; j < formal_arr->dims.size(); ++j) {
          if (formal_arr->dims[j] != actual_arr->dims[j]) {
            std::exit(4);
          }
        }
      }
    } else {
      if (!formal->compatible(actual)) {
        std::exit(4);
      }
    }
  }

  node->symbol = symbol;
  return func_type->return_type;
}

TypePtr TypeChecker::checkUnaryExp(AST::UnaryExpPtr node) {
  auto type = check(node->exp);
  // 一元表达式只支持 int 类型，因此你需要判断 type 是否为 int
  if (!type->compatible(PrimitiveType::Int)) {
    std::cerr << "Semantic Error at line " << node->lineno
              << ": invalid operand to unary expression\n";
    std::exit(5);
  }
  return PrimitiveType::Int;
}

TypePtr TypeChecker::checkBinaryExp(AST::BinaryExpPtr node) {
  TypePtr left_type = check(node->left);
  TypePtr right_type = check(node->right);
  // 二元表达式只支持 int 类型，因此你需要判断左右表达式的类型是否为 int
  if (!left_type->compatible(PrimitiveType::Int) ||
      !right_type->compatible(PrimitiveType::Int)) {
    std::cerr << "Semantic Error at line " << node->lineno
              << ": invalid operands to binary expression\n";
    std::exit(5);
  }

  return PrimitiveType::Int;
}
