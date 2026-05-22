#include "ir_translator.hpp"

#include <cassert>

static void append_code(IR::Code& dst, IR::Code& src) {
  std::move(src.begin(), src.end(), std::back_inserter(dst));
}

static std::vector<int> get_array_dims_from_symbol(const SymbolPtr& symbol) {
  if (!symbol || !symbol->type) {
    return {};
  }

  auto arr = std::dynamic_pointer_cast<ArrayType>(symbol->type);
  if (!arr) {
    return {};
  }

  return arr->dims;
}

static int total_elems(const std::vector<int>& dims) {
  int total = 1;
  for (int d : dims) {
    total *= d;
  }
  return total;
}

static int product_from(const std::vector<int>& dims, int start) {
  int result = 1;
  for (int i = start; i < (int)dims.size(); i++) {
    result *= dims[i];
  }
  return result;
}

static int brace_depth(AST::InitValPtr init) {
  if (!init || !init->is_list) return 0;

  int best = 0;
  for (auto& elem : init->elements) {
    best = std::max(best, brace_depth(elem));
  }
  return best + 1;
}

static void fill_init_range(AST::InitValPtr init,
                            const std::vector<int>& dims,
                            int start,
                            int end,
                            std::vector<AST::NodePtr>& out) {
  if (!init) return;

  int pos = start;

  // Scalar initializer.
  if (!init->is_list) {
    if (init->expr && pos < end && pos < (int)out.size()) {
      out[pos] = init->expr;
    }
    return;
  }

  for (auto& elem : init->elements) {
    if (pos >= end || pos >= (int)out.size()) break;

    if (!elem->is_list) {
      if (elem->expr) {
        out[pos++] = elem->expr;
      }
      continue;
    }

    /*
      A braced sub-list should initialize the next aligned sub-aggregate.

      Example:
        int a[2][3][4] =
          {1,2,3,4, {5}, {9}, 13,14,15,16, {17}, {21}};

      After 1,2,3,4, pos = 4.
      {5} should occupy positions 4..7, not 12..23.
      So choose sub_size by brace depth:
        {5}        depth 1 -> last dimension size = 4
        {{...}}    depth 2 -> last two dimensions size = 3*4
    */
    int depth = brace_depth(elem);
    if (depth <= 0) depth = 1;
    if (depth > (int)dims.size()) depth = dims.size();

    int sub_start_dim = (int)dims.size() - depth;
    int sub_size = product_from(dims, sub_start_dim);

    if (sub_size <= 0) sub_size = 1;

    // Align to the next sub-aggregate boundary.
    int rel = pos - start;
    if (rel % sub_size != 0) {
      pos += sub_size - (rel % sub_size);
    }

    if (pos >= end || pos >= (int)out.size()) break;

    int sub_start = pos;
    int sub_end = std::min(sub_start + sub_size, end);

    fill_init_range(elem, dims, sub_start, sub_end, out);

    // Consume the whole sub-aggregate. Missing values remain zero.
    pos = sub_start + sub_size;
  }
}

static std::vector<AST::NodePtr> flatten_array_init(AST::InitValPtr init,
                                                    const std::vector<int>& dims) {
  int total = product_from(dims, 0);
  std::vector<AST::NodePtr> out(total, nullptr);
  fill_init_range(init, dims, 0, total, out);
  return out;
}

static std::string ir_name(const SymbolPtr& symbol, const std::string& fallback) {
  if (symbol && !symbol->unique_name.empty()) {
    return symbol->unique_name;
  }
  return fallback;
}

std::string IRTranslator::new_temp() {
  static int temp_count = 0;
  return "T" + std::to_string(temp_count++);
}

std::string IRTranslator::new_label() {
  static int label_count = 0;
  return "L" + std::to_string(label_count++);
}

static bool is_array_symbol(const SymbolPtr& symbol) {
  if (!symbol || !symbol->type) return false;
  return std::dynamic_pointer_cast<ArrayType>(symbol->type) != nullptr;
}

static int const_expr_value(AST::NodePtr node) {
  if (auto c = std::dynamic_pointer_cast<AST::IntConst>(node)) {
    return c->value;
  }

  // Global initializers in this lab are usually constants.
  // If you need more later, add constant folding here.
  return 0;
}

static std::vector<int> flatten_global_init_values(AST::InitValPtr init,
                                                   const std::vector<int>& dims) {
  std::vector<AST::NodePtr> nodes = flatten_array_init(init, dims);
  std::vector<int> values;

  for (auto& n : nodes) {
    values.push_back(n ? const_expr_value(n) : 0);
  }

  return values;
}


IR::Code IRTranslator::translate(AST::NodePtr node) {
#define TRANSLATE_NODE(type)                                 \
  if (auto n = std::dynamic_pointer_cast<AST::type>(node)) { \
    return translate##type(n);                               \
  }
  // 递归翻译 AST 的每个节点
  // 如果你添加了新的 AST 节点类型，记得在这里添加对应的翻译函数

  TRANSLATE_NODE(CompUnit)
  TRANSLATE_NODE(FuncDef)
  TRANSLATE_NODE(Block)
  TRANSLATE_NODE(VarDecl)
  TRANSLATE_NODE(VarDef)
  TRANSLATE_NODE(AssignStmt)
  TRANSLATE_NODE(ReturnStmt)
  TRANSLATE_NODE(LVal)
  TRANSLATE_NODE(BinaryExp)
  TRANSLATE_NODE(UnaryExp)
  TRANSLATE_NODE(FuncCall)
  TRANSLATE_NODE(IntConst)
  TRANSLATE_NODE(ExprStmt)
  TRANSLATE_NODE(IfStmt)
  TRANSLATE_NODE(WhileStmt)

//#warning Add more AST node types if needed

#undef TRANSLATE_NODE

  ASSERT(false,
         "Unknown AST node type " + node->to_string() + " in IR translation");
}

IR::Code IRTranslator::translateExp(AST::NodePtr node,
                                    const std::string& place) {
#define TRANSLATE_EXP_NODE(type)                             \
  if (auto n = std::dynamic_pointer_cast<AST::type>(node)) { \
    return translate##type(n, place);                        \
  }

  TRANSLATE_EXP_NODE(BinaryExp)
  TRANSLATE_EXP_NODE(UnaryExp)
  TRANSLATE_EXP_NODE(FuncCall)
  TRANSLATE_EXP_NODE(IntConst)
  TRANSLATE_EXP_NODE(LVal)

//#warning Add more AST node types if needed

#undef TRANSLATE_EXP_NODE

  ASSERT(false, "No translateExp for node " + node->to_string());
}

IR::Code IRTranslator::translateCompUnit(AST::CompUnitPtr node) {
  IR::Code ir;

  // First pass: emit global variables before functions.
  in_global_scope = true;

  for (auto& unit : node->units) {
    if (auto decl = std::dynamic_pointer_cast<AST::VarDecl>(unit)) {
      auto global_ir = translateGlobalVarDecl(decl);
      append_code(ir, global_ir);
    }
  }

  in_global_scope = false;

  // Second pass: emit functions.
  for (auto& unit : node->units) {
    if (std::dynamic_pointer_cast<AST::VarDecl>(unit)) {
      continue;
    }

    auto unit_ir = translate(unit);
    append_code(ir, unit_ir);
  }

  return ir;
}

IR::Code IRTranslator::translateGlobalVarDecl(AST::VarDeclPtr node) {
  IR::Code ir;

  for (auto& def : node->defs) {
    auto def_ir = translateGlobalVarDef(def);
    append_code(ir, def_ir);
  }

  return ir;
}

IR::Code IRTranslator::translateGlobalVarDef(AST::VarDefPtr node) {
  IR::Code ir;

  std::string name = ir_name(node->symbol, node->ident);
  global_symbols.insert(name);

  // Global array
  if (!node->dims.empty()) {
    int elems = total_elems(node->dims);
    std::vector<int> values(elems, 0);

    if (node->init) {
      values = flatten_global_init_values(node->init, node->dims);
      if ((int)values.size() < elems) {
        values.resize(elems, 0);
      }
    }

    ir.push_back(IR::Global::create(name, elems * 4, values));
    return ir;
  }

  // Global scalar: treat as one int = 4 bytes
  int init_value = 0;
  if (node->init && !node->init->is_list && node->init->expr) {
    init_value = const_expr_value(node->init->expr);
  }

  ir.push_back(IR::Global::create(name, 4, std::vector<int>{init_value}));
  return ir;
}

// IR::Code IRTranslator::translateFuncDef(AST::FuncDefPtr node) {
//   IR::Code ir;
//   ir.push_back(IR::Function::create(node->name));
//   auto block_ir = translate(node->block);
//   std::move(block_ir.begin(), block_ir.end(), std::back_inserter(ir));
//   return ir;
// }

IR::Code IRTranslator::translateLValAddr(AST::LValPtr node,
                                         const std::string& addr_place) {
  IR::Code ir;

  std::string base_name = ir_name(node->symbol, node->ident);
  bool is_global = global_symbols.count(base_name) > 0;

  std::string base = base_name;

  // Global variable/array: get address first.
  if (is_global) {
    base = new_temp();
    ir.push_back(IR::LoadAddr::create(base, base_name));
  }

  // No index: address of variable/array itself.
  if (node->indices.empty()) {
    if (!addr_place.empty()) {
      ir.push_back(IR::Assign::create(addr_place, base));
    }
    return ir;
  }

  std::vector<int> dims = get_array_dims_from_symbol(node->symbol);

  std::string offset_elems;

  for (int i = 0; i < (int)node->indices.size(); i++) {
    auto idx_place = new_temp();
    auto idx_ir = translateExp(node->indices[i], idx_place);
    append_code(ir, idx_ir);

    int stride = 1;
    for (int j = i + 1; j < (int)dims.size(); j++) {
      stride *= dims[j];
    }

    auto term = new_temp();
    if (stride == 1) {
      ir.push_back(IR::Assign::create(term, idx_place));
    } else {
      ir.push_back(IR::BinaryImm::create(term, idx_place, BinaryOp::Mul, stride));
    }

    if (offset_elems.empty()) {
      offset_elems = term;
    } else {
      auto sum = new_temp();
      ir.push_back(IR::Binary::create(sum, offset_elems, BinaryOp::Add, term));
      offset_elems = sum;
    }
  }

  auto byte_offset = new_temp();
  ir.push_back(IR::BinaryImm::create(byte_offset, offset_elems, BinaryOp::Mul, 4));
  ir.push_back(IR::Binary::create(addr_place, base, BinaryOp::Add, byte_offset));

  return ir;
}

IR::Code IRTranslator::translateFuncDef(AST::FuncDefPtr node) {
  IR::Code ir;

  ir.push_back(IR::Function::create(node->name));

  for (auto& param : node->params) {
    std::string pname = ir_name(param->symbol, param->ident);
    ir.push_back(IR::Param::create(pname));
  }

  auto block_ir = translate(node->block);
  append_code(ir, block_ir);

  // SysY allows void functions to omit "return;"
  // But Zero IR interpreter requires every function to return.
  if (node->return_btype == BasicType::Void) {
    bool has_tail_return_or_goto = false;

    if (!ir.empty()) {
      auto last = ir.back();

      if (std::dynamic_pointer_cast<IR::Return>(last) ||
          std::dynamic_pointer_cast<IR::Goto>(last)) {
        has_tail_return_or_goto = true;
      }
    }

    if (!has_tail_return_or_goto) {
      ir.push_back(IR::Return::create());
    }
  }

  return ir;
}

IR::Code IRTranslator::translateBlock(AST::BlockPtr node) {
  IR::Code ir;
  for (auto& stmt : node->stmts) {
    auto stmt_ir = translate(stmt);
    std::move(stmt_ir.begin(), stmt_ir.end(), std::back_inserter(ir));
  }
  return ir;
}

IR::Code IRTranslator::translateVarDecl(AST::VarDeclPtr node) {
  IR::Code ir;
  for (auto& def : node->defs) {
    auto def_ir = translate(def);
    std::move(def_ir.begin(), def_ir.end(), std::back_inserter(ir));
  }
  return ir;
}

IR::Code IRTranslator::translateVarDef(AST::VarDefPtr node) {
  IR::Code ir;

  std::string name = ir_name(node->symbol, node->ident);

  // array declaration
  if (!node->dims.empty()) {
    int elems = total_elems(node->dims);
    ir.push_back(IR::Dec::create(name, elems * 4));

    if (node->init) {
      std::vector<AST::NodePtr> values = flatten_array_init(node->init, node->dims);

      for (int i = 0; i < elems; i++) {
        auto val = new_temp();
        auto addr = new_temp();

        if (values[i]) {
          auto val_ir = translateExp(values[i], val);
          append_code(ir, val_ir);
        } else {
          ir.push_back(IR::LoadImm::create(val, 0));
        }

        ir.push_back(IR::BinaryImm::create(addr, name, BinaryOp::Add, i * 4));
        ir.push_back(IR::Store::create(addr, val));
      }
    }

    return ir;
  }

  // scalar variable
  if (node->init && !node->init->is_list && node->init->expr) {
    auto init_ir = translateExp(node->init->expr, name);
    append_code(ir, init_ir);
  }

  return ir;
}

IR::Code IRTranslator::translateAssignStmt(AST::AssignStmtPtr node) {
  IR::Code ir;

  std::string lhs_name = ir_name(node->lval->symbol, node->lval->ident);
  bool is_global = global_symbols.count(lhs_name) > 0;
  bool lhs_is_array = is_array_symbol(node->lval->symbol);

  // Local scalar assignment
  if (node->lval->indices.empty() && !is_global && !lhs_is_array) {
    auto r_ir = translateExp(node->exp, lhs_name);
    append_code(ir, r_ir);
    return ir;
  }

  // Global scalar or array element assignment
  auto rhs = new_temp();
  auto rhs_ir = translateExp(node->exp, rhs);
  append_code(ir, rhs_ir);

  auto addr = new_temp();
  auto addr_ir = translateLValAddr(node->lval, addr);
  append_code(ir, addr_ir);

  ir.push_back(IR::Store::create(addr, rhs));
  return ir;
}

IR::Code IRTranslator::translateReturnStmt(AST::ReturnStmtPtr node) {
  IR::Code ir;

  // 翻译返回值
  // 如果有返回值，则：
  // place = new_temp();
  // auto exp_ir = translateExp(node->exp, place);
  // return exp_ir + [RETURN place];
  // 否则：
  // return [RETURN];

  if (node->exp) {
    auto place = new_temp();
    auto exp_ir = translateExp(node->exp, place);
    append_code(ir, exp_ir);
    ir.push_back(IR::Return::create(place));
  } else {
    ir.push_back(IR::Return::create());
  }
//#warning Not implemented: IRTranslator::translateReturnStmt

  return ir;
}

IR::Code IRTranslator::translateLVal(AST::LValPtr node,
                                     const std::string& place) {
  IR::Code ir;

  std::string name = ir_name(node->symbol, node->ident);
  bool is_global = global_symbols.count(name) > 0;
  bool is_array = is_array_symbol(node->symbol);

  // Local scalar
  if (node->indices.empty() && !is_global && !is_array) {
    if (!place.empty()) {
      ir.push_back(IR::Assign::create(place, name));
    }
    return ir;
  }

  // Plain local array name: address
  if (node->indices.empty() && !is_global && is_array) {
    if (!place.empty()) {
      ir.push_back(IR::Assign::create(place, name));
    }
    return ir;
  }

  auto addr = new_temp();
  auto addr_ir = translateLValAddr(node, addr);
  append_code(ir, addr_ir);

  std::vector<int> dims = get_array_dims_from_symbol(node->symbol);

  // Partial array indexing gives address.
  // Example: int a[4][2]; a[2] is address of row 2.
  if (is_array && (int)node->indices.size() < (int)dims.size()) {
    if (!place.empty()) {
      ir.push_back(IR::Assign::create(place, addr));
    }
    return ir;
  }

  // Global scalar or full array element access: load value.
  if (!place.empty()) {
    ir.push_back(IR::Deref::create(place, addr));
  }

  return ir;
}

IR::Code IRTranslator::translateBinaryExp(AST::BinaryExpPtr node,
                                          const std::string& place) {
  IR::Code ir;

  // Keep C/SysY short-circuit semantics for logical operators even when used
  // as expressions (not only in conditions).
  if (node->op == BinaryOp::LAnd || node->op == BinaryOp::LOr) {
    if (place.empty()) {
      auto sink = new_temp();
      auto logical_ir = translateBinaryExp(node, sink);
      append_code(ir, logical_ir);
      return ir;
    }

    auto true_label = new_label();
    auto false_label = new_label();
    auto end_label = new_label();

    auto cond_ir = translateCond(node, true_label, false_label);
    append_code(ir, cond_ir);

    ir.push_back(IR::Label::create(true_label));
    ir.push_back(IR::LoadImm::create(place, 1));
    ir.push_back(IR::Goto::create(end_label));

    ir.push_back(IR::Label::create(false_label));
    ir.push_back(IR::LoadImm::create(place, 0));
    ir.push_back(IR::Goto::create(end_label));

    ir.push_back(IR::Label::create(end_label));
    return ir;
  }

  auto left_place = new_temp();
  auto right_place = new_temp();

  // 翻译左右子表达式
  auto left_ir = translateExp(node->left, left_place);
  auto right_ir = translateExp(node->right, right_place);

  std::move(left_ir.begin(), left_ir.end(), std::back_inserter(ir));
  std::move(right_ir.begin(), right_ir.end(), std::back_inserter(ir));

  // 添加二元运算指令
  if (!place.empty()) {
    ir.push_back(IR::Binary::create(place, left_place, node->op, right_place));
  }
  return ir;
}

IR::Code IRTranslator::translateUnaryExp(AST::UnaryExpPtr node,
                                         const std::string& place) {
  IR::Code ir;

  auto exp_place = new_temp();
  auto exp_ir = translateExp(node->exp, exp_place);
  append_code(ir, exp_ir);

  if (place.empty()) {
    return ir;
  }

  // +x
  if (node->op == UnaryOp::Pos) {
    ir.push_back(IR::Assign::create(place, exp_place));
    return ir;
  }

  // -x
  if (node->op == UnaryOp::Neg) {
    ir.push_back(IR::Unary::create(place, node->op, exp_place));
    return ir;
  }

  if (node->op == UnaryOp::Not) {
    auto zero = new_temp();
    auto true_label = new_label();
    auto false_label = new_label();
    auto end_label = new_label();

    ir.push_back(IR::LoadImm::create(zero, 0));
    ir.push_back(IR::If::create(exp_place, BinaryOp::Eq, zero, true_label));
    ir.push_back(IR::Goto::create(false_label));

    ir.push_back(IR::Label::create(true_label));
    ir.push_back(IR::LoadImm::create(place, 1));
    ir.push_back(IR::Goto::create(end_label));

    ir.push_back(IR::Label::create(false_label));
    ir.push_back(IR::LoadImm::create(place, 0));
    ir.push_back(IR::Goto::create(end_label));

    ir.push_back(IR::Label::create(end_label));
    return ir;
  }

  ASSERT(false, "Unknown unary operator");
}

IR::Code IRTranslator::translateFuncCall(AST::FuncCallPtr node,
                                         const std::string& place) {
  IR::Code ir;
  std::vector<std::string> arg_places;

  // 首先翻译参数表达式，并存在临时变量中
  // 接下来，添加参数传递指令和函数调用指令
  // 如果 place 不为空，则将函数调用的返回值赋给 place
  for (auto& arg : node->args) {
    auto arg_place = new_temp();
    auto arg_ir = translateExp(arg, arg_place);
    append_code(ir, arg_ir);
    arg_places.push_back(arg_place);
  }

  for (int i = 0; i < (int)arg_places.size(); i++) {
    ir.push_back(IR::Arg::create(arg_places[i], node->name, i));
  }

  if (!place.empty()) {
    ir.push_back(IR::Call::create(place, node->name));
  } else {
    ir.push_back(IR::Call::create(node->name));
  }
//#warning Not implemented: IRTranslator::translateFuncCall

  return ir;
}

IR::Code IRTranslator::translateIntConst(AST::IntConstPtr node,
                                         const std::string& place) {
  IR::Code ir;
  // 添加赋值常量指令
  if (!place.empty()) {
    ir.push_back(IR::LoadImm::create(place, node->value));
  }
  return ir;
}

IR::Code IRTranslator::translateExprStmt(AST::ExprStmtPtr node) {
  IR::Code ir;

  if (node->exp) {
    auto exp_ir = translateExp(node->exp, "");
    append_code(ir, exp_ir);
  }

  return ir;
}

IR::Code IRTranslator::translateCond(AST::NodePtr node,
                                     const std::string& true_label,
                                     const std::string& false_label) {
  IR::Code ir;

  if (auto bin = std::dynamic_pointer_cast<AST::BinaryExp>(node)) {
    if (bin->op == BinaryOp::LAnd) {
      auto mid_label = new_label();
      auto left_ir = translateCond(bin->left, mid_label, false_label);
      append_code(ir, left_ir);
      ir.push_back(IR::Label::create(mid_label));
      auto right_ir = translateCond(bin->right, true_label, false_label);
      append_code(ir, right_ir);
      return ir;
    }

    if (bin->op == BinaryOp::LOr) {
      auto mid_label = new_label();
      auto left_ir = translateCond(bin->left, true_label, mid_label);
      append_code(ir, left_ir);
      ir.push_back(IR::Label::create(mid_label));
      auto right_ir = translateCond(bin->right, true_label, false_label);
      append_code(ir, right_ir);
      return ir;
    }

    if (bin->op == BinaryOp::Eq || bin->op == BinaryOp::Ne ||
        bin->op == BinaryOp::Lt || bin->op == BinaryOp::Le ||
        bin->op == BinaryOp::Gt || bin->op == BinaryOp::Ge) {
      auto left_place = new_temp();
      auto right_place = new_temp();

      auto left_ir = translateExp(bin->left, left_place);
      auto right_ir = translateExp(bin->right, right_place);

      append_code(ir, left_ir);
      append_code(ir, right_ir);

      ir.push_back(IR::If::create(left_place, bin->op, right_place, true_label));
      ir.push_back(IR::Goto::create(false_label));
      return ir;
    }
  }

  auto place = new_temp();
  auto zero = new_temp();

  auto exp_ir = translateExp(node, place);
  append_code(ir, exp_ir);

  ir.push_back(IR::LoadImm::create(zero, 0));
  ir.push_back(IR::If::create(place, BinaryOp::Ne, zero, true_label));
  ir.push_back(IR::Goto::create(false_label));

  return ir;
}

IR::Code IRTranslator::translateIfStmt(AST::IfStmtPtr node) {
  IR::Code ir;

  auto true_label = new_label();
  auto false_label = new_label();
  auto end_label = new_label();

  auto cond_ir = translateCond(node->cond, true_label, false_label);
  append_code(ir, cond_ir);

  ir.push_back(IR::Label::create(true_label));
  auto then_ir = translate(node->then_stmt);
  append_code(ir, then_ir);
  ir.push_back(IR::Goto::create(end_label));

  ir.push_back(IR::Label::create(false_label));
  if (node->else_stmt) {
    auto else_ir = translate(node->else_stmt);
    append_code(ir, else_ir);
  }
  ir.push_back(IR::Goto::create(end_label));

  ir.push_back(IR::Label::create(end_label));

  return ir;
}

IR::Code IRTranslator::translateWhileStmt(AST::WhileStmtPtr node) {
  IR::Code ir;

  auto cond_label = new_label();
  auto body_label = new_label();
  auto end_label = new_label();

  ir.push_back(IR::Label::create(cond_label));

  auto cond_ir = translateCond(node->cond, body_label, end_label);
  append_code(ir, cond_ir);

  ir.push_back(IR::Label::create(body_label));
  auto body_ir = translate(node->body);
  append_code(ir, body_ir);
  ir.push_back(IR::Goto::create(cond_label));

  ir.push_back(IR::Label::create(end_label));

  return ir;
}
