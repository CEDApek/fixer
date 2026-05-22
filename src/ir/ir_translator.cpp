#include "ir_translator.hpp"

#include <cassert>
#include <unordered_set>
#include <cstdlib>
#include <iostream>

static bool ir_debug_enabled() {
  return std::getenv("IR_DEBUG") != nullptr;
}

static int ir_debug_depth = 0;

static void ir_debug_indent() {
  for (int i = 0; i < ir_debug_depth; i++) {
    std::cerr << "  ";
  }
}

struct IRDebugScope {
  std::string name;

  IRDebugScope(const std::string& name, const std::string& info = "")
      : name(name) {
    if (!ir_debug_enabled()) return;

    ir_debug_indent();
    std::cerr << ">> " << name;
    if (!info.empty()) std::cerr << " : " << info;
    std::cerr << "\n";

    ir_debug_depth++;
  }

  ~IRDebugScope() {
    if (!ir_debug_enabled()) return;

    ir_debug_depth--;
    ir_debug_indent();
    std::cerr << "<< " << name << "\n";
  }
};

static void dump_ir_code(const std::string& name, const IR::Code& code) {
  if (!ir_debug_enabled()) return;

  ir_debug_indent();
  std::cerr << "[IR] " << name << " generated " << code.size() << " instrs\n";

  int count = 0;
  for (auto& inst : code) {
    ir_debug_indent();
    std::cerr << "  " << inst->to_string() << "\n";

    count++;
    if (count >= 30) {
      ir_debug_indent();
      std::cerr << "  ... snipped ...\n";
      break;
    }
  }
}

static void append_code(IR::Code& dst, IR::Code& src) {
  std::move(src.begin(), src.end(), std::back_inserter(dst));
}

static bool stmt_must_exit(AST::NodePtr node) {
  if (!node) return false;

  if (std::dynamic_pointer_cast<AST::ReturnStmt>(node)) {
    return true;
  }

  if (auto block = std::dynamic_pointer_cast<AST::Block>(node)) {
    for (auto& stmt : block->stmts) {
      if (stmt_must_exit(stmt)) return true;
    }
    return false;
  }

  if (auto ifs = std::dynamic_pointer_cast<AST::IfStmt>(node)) {
    if (!ifs->else_stmt) return false;
    return stmt_must_exit(ifs->then_stmt) &&
           stmt_must_exit(ifs->else_stmt);
  }

  return false;
}

static bool is_array_symbol(const SymbolPtr& symbol);
static std::string ir_name(const SymbolPtr& symbol,
                           const std::string& fallback);

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

static bool is_relop(BinaryOp op) {
  return op == BinaryOp::Eq || op == BinaryOp::Ne ||
         op == BinaryOp::Lt || op == BinaryOp::Le ||
         op == BinaryOp::Gt || op == BinaryOp::Ge;
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

static bool ends_with_return_or_goto(const IR::Code& code) {
  if (code.empty()) return false;
  auto last = code.back();
  return std::dynamic_pointer_cast<IR::Return>(last) ||
         std::dynamic_pointer_cast<IR::Goto>(last);
}

static int brace_depth(AST::InitValPtr init) {
  if (!init || !init->is_list) return 0;

  int best = 0;
  for (auto& elem : init->elements) {
    best = std::max(best, brace_depth(elem));
  }
  return best + 1;
}

static bool stmt_always_returns(AST::NodePtr node) {
  if (!node) return false;

  if (std::dynamic_pointer_cast<AST::ReturnStmt>(node)) {
    return true;
  }

  if (auto block = std::dynamic_pointer_cast<AST::Block>(node)) {
    for (auto& stmt : block->stmts) {
      if (stmt_always_returns(stmt)) {
        return true;
      }
    }
    return false;
  }

  if (auto ifs = std::dynamic_pointer_cast<AST::IfStmt>(node)) {
    if (!ifs->else_stmt) return false;
    return stmt_always_returns(ifs->then_stmt) &&
           stmt_always_returns(ifs->else_stmt);
  }

  return false;
}

static bool eval_const_int(AST::NodePtr node, int& out) {
  if (!node) return false;

  if (auto c = std::dynamic_pointer_cast<AST::IntConst>(node)) {
    out = c->value;
    return true;
  }

  if (auto u = std::dynamic_pointer_cast<AST::UnaryExp>(node)) {
    int v;
    if (!eval_const_int(u->exp, v)) return false;

    if (u->op == UnaryOp::Pos) out = v;
    else if (u->op == UnaryOp::Neg) out = -v;
    else if (u->op == UnaryOp::Not) out = !v;
    else return false;

    return true;
  }

  if (auto b = std::dynamic_pointer_cast<AST::BinaryExp>(node)) {
    int l, r;
    if (!eval_const_int(b->left, l)) return false;
    if (!eval_const_int(b->right, r)) return false;

    switch (b->op) {
      case BinaryOp::Add: out = l + r; return true;
      case BinaryOp::Sub: out = l - r; return true;
      case BinaryOp::Mul: out = l * r; return true;
      case BinaryOp::Div: if (r == 0) return false; out = l / r; return true;
      case BinaryOp::Mod: if (r == 0) return false; out = l % r; return true;

      case BinaryOp::Eq: out = l == r; return true;
      case BinaryOp::Ne: out = l != r; return true;
      case BinaryOp::Lt: out = l < r; return true;
      case BinaryOp::Le: out = l <= r; return true;
      case BinaryOp::Gt: out = l > r; return true;
      case BinaryOp::Ge: out = l >= r; return true;

      case BinaryOp::LAnd: out = l && r; return true;
      case BinaryOp::LOr: out = l || r; return true;

      default: return false;
    }
  }

  return false;
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

// static AST::ReturnStmtPtr as_return_stmt(AST::NodePtr node) {
//   if (!node) return nullptr;

//   if (auto ret = std::dynamic_pointer_cast<AST::ReturnStmt>(node)) {
//     return ret;
//   }

//   if (auto block = std::dynamic_pointer_cast<AST::Block>(node)) {
//     if (block->stmts.size() == 1) {
//       return std::dynamic_pointer_cast<AST::ReturnStmt>(block->stmts[0]);
//     }
//   }

//   return nullptr;
// }

static bool contains_direct_call(AST::NodePtr node, const std::string& name) {
  if (!node) return false;

  if (auto call = std::dynamic_pointer_cast<AST::FuncCall>(node)) {
    return call->name == name;
  }

  for (auto& child : node->get_children()) {
    if (contains_direct_call(child, name)) return true;
  }

  return false;
}

static bool contains_while(AST::NodePtr node) {
  if (!node) return false;

  if (std::dynamic_pointer_cast<AST::WhileStmt>(node)) return true;

  for (auto& child : node->get_children()) {
    if (contains_while(child)) return true;
  }

  return false;
}

static bool contains_local_array(AST::NodePtr node) {
  if (!node) return false;

  if (auto def = std::dynamic_pointer_cast<AST::VarDef>(node)) {
    if (!def->dims.empty()) return true;
  }

  for (auto& child : node->get_children()) {
    if (contains_local_array(child)) return true;
  }

  return false;
}

static bool assigns_scalar_param(AST::NodePtr node,
                                 const std::unordered_set<std::string>& params) {
  if (!node) return false;

  if (auto asg = std::dynamic_pointer_cast<AST::AssignStmt>(node)) {
    if (asg->lval && asg->lval->indices.empty()) {
      std::string lhs = ir_name(asg->lval->symbol, asg->lval->ident);
      if (params.count(lhs)) return true;
    }
  }

  for (auto& child : node->get_children()) {
    if (assigns_scalar_param(child, params)) return true;
  }

  return false;
}

static bool contains_func_call(AST::NodePtr node) {
  if (!node) return false;

  if (std::dynamic_pointer_cast<AST::FuncCall>(node)) {
    return true;
  }

  for (auto& child : node->get_children()) {
    if (contains_func_call(child)) return true;
  }

  return false;
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

std::string IRTranslator::getConstTemp(int value) {
  auto it = const_cache.find(value);
  if (it != const_cache.end()) {
    return it->second;
  }

  std::string temp = new_temp();
  const_cache[value] = temp;
  return temp;
}

bool IRTranslator::isInlining() const {
  return !inline_return_stack.empty();
}

bool IRTranslator::isInlineRecursive(const std::string& name) const {
  for (auto& f : inline_func_stack) {
    if (f == name) return true;
  }
  return false;
}

std::string IRTranslator::resolveAlias(const std::string& name) const {
  for (auto it = alias_stack.rbegin(); it != alias_stack.rend(); ++it) {
    auto found = it->find(name);
    if (found != it->end()) {
      return found->second;
    }
  }
  return name;
}

IR::Code IRTranslator::translateValueName(AST::NodePtr node,
                                          std::string& out) {
  IRDebugScope dbg("translateValueName", node ? node->to_string() : "null");
  IR::Code ir;

  if (auto c = std::dynamic_pointer_cast<AST::IntConst>(node)) {
    std::cerr << "[VALUE KIND] int const " << c->value << "\n";
    out = new_temp();
    ir.push_back(IR::LoadImm::create(out, c->value));
    dump_ir_code("translateValueName", ir);return ir;
  }

  if (auto lv = std::dynamic_pointer_cast<AST::LVal>(node)) {
    std::cerr << "[VALUE KIND] lval " << lv->ident
              << ", indices=" << lv->indices.size() << "\n";
    std::string name = resolveAlias(ir_name(lv->symbol, lv->ident));
    bool is_global = global_symbols.count(name) > 0;
    bool is_array = is_array_symbol(lv->symbol);

    if (lv->indices.empty()) {
      if (!is_global) {
        out = name;
        dump_ir_code("translateValueName", ir);return ir;
      }

      if (is_array) {
        out = new_temp();
        ir.push_back(IR::LoadAddr::create(out, name));
        dump_ir_code("translateValueName", ir);return ir;
      }

      auto addr = new_temp();
      out = new_temp();
      ir.push_back(IR::LoadAddr::create(addr, name));
      ir.push_back(IR::Deref::create(out, addr));
      dump_ir_code("translateValueName", ir);return ir;
    }
  }

  // IMPORTANT: judgment/logical expression used as value
  if (auto bin = std::dynamic_pointer_cast<AST::BinaryExp>(node)) {
      std::cerr << "[VALUE KIND] binary expression op="
            << op_to_string(bin->op) << "\n";
    if (bin->op == BinaryOp::LAnd || bin->op == BinaryOp::LOr ||
        is_relop(bin->op)) {
      out = new_temp();
      auto bool_ir = translateBoolValue(node, out);
      append_code(ir, bool_ir);
      dump_ir_code("translateValueName", ir);return ir;
    }
  }

  out = new_temp();
  auto exp_ir = translateExp(node, out);
  append_code(ir, exp_ir);
  dump_ir_code("translateValueName", ir);return ir;
}

std::string IRTranslator::getGlobalAddrTemp(const std::string& name) {
  auto it = global_addr_cache.find(name);
  if (it != global_addr_cache.end()) {
    return it->second;
  }

  std::string temp = new_temp();
  global_addr_cache[name] = temp;
  return temp;
}

static bool is_array_symbol(const SymbolPtr& symbol) {
  if (!symbol || !symbol->type) return false;
  return std::dynamic_pointer_cast<ArrayType>(symbol->type) != nullptr;
}

static int const_expr_value(AST::NodePtr node) {
  if (!node) return 0;

  if (auto c = std::dynamic_pointer_cast<AST::IntConst>(node)) {
    return c->value;
  }

  if (auto u = std::dynamic_pointer_cast<AST::UnaryExp>(node)) {
    int v = const_expr_value(u->exp);

    if (u->op == UnaryOp::Pos) return v;
    if (u->op == UnaryOp::Neg) return -v;
    if (u->op == UnaryOp::Not) return !v;

    return 0;
  }

  if (auto b = std::dynamic_pointer_cast<AST::BinaryExp>(node)) {
    int l = const_expr_value(b->left);
    int r = const_expr_value(b->right);

    switch (b->op) {
      case BinaryOp::Add: return l + r;
      case BinaryOp::Sub: return l - r;
      case BinaryOp::Mul: return l * r;
      case BinaryOp::Div: return r == 0 ? 0 : l / r;
      case BinaryOp::Mod: return r == 0 ? 0 : l % r;

      case BinaryOp::Eq: return l == r;
      case BinaryOp::Ne: return l != r;
      case BinaryOp::Lt: return l < r;
      case BinaryOp::Le: return l <= r;
      case BinaryOp::Gt: return l > r;
      case BinaryOp::Ge: return l >= r;

      case BinaryOp::LAnd: return l && r;
      case BinaryOp::LOr: return l || r;

      default: return 0;
    }
  }

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

  func_defs.clear();

  for (auto& unit : node->units) {
    if (auto func = std::dynamic_pointer_cast<AST::FuncDef>(unit)) {
      func_defs[func->name] = func;
    }
  }

  // existing global pass
  in_global_scope = true;
  for (auto& unit : node->units) {
    if (auto decl = std::dynamic_pointer_cast<AST::VarDecl>(unit)) {
      auto global_ir = translateGlobalVarDecl(decl);
      append_code(ir, global_ir);
    }
  }
  in_global_scope = false;

  // existing function pass
  for (auto& unit : node->units) {
    if (std::dynamic_pointer_cast<AST::VarDecl>(unit)) continue;

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
  IRDebugScope dbg("translateLValAddr", node->to_string());
  std::cerr << "[ADDR] " << node->ident
            << ", indices=" << node->indices.size() << "\n";
  IR::Code ir;

  std::string base_name = resolveAlias(ir_name(node->symbol, node->ident));
  bool is_global = global_symbols.count(base_name) > 0;

  std::string base = base_name;
  if (is_global) {
    base = new_temp();
    ir.push_back(IR::LoadAddr::create(base, base_name));
  }

  if (node->indices.empty()) {
    if (!addr_place.empty() && addr_place != base) {
      ir.push_back(IR::Assign::create(addr_place, base));
    }
    return ir;
  }

  std::vector<int> dims = get_array_dims_from_symbol(node->symbol);

  int const_offset_bytes = 0;
  std::string dynamic_offset_bytes;

  for (int i = 0; i < (int)node->indices.size(); i++) {
    int stride = 1;
    for (int j = i + 1; j < (int)dims.size(); j++) {
      stride *= dims[j];
    }

    int scale_bytes = stride * 4;

    if (auto c = std::dynamic_pointer_cast<AST::IntConst>(node->indices[i])) {
      const_offset_bytes += c->value * scale_bytes;
      continue;
    }

    std::string idx_name;
    auto idx_ir = translateValueName(node->indices[i], idx_name);
    append_code(ir, idx_ir);

    std::string term = idx_name;
    if (scale_bytes != 1) {
      term = new_temp();
      ir.push_back(IR::BinaryImm::create(term, idx_name,
                                         BinaryOp::Mul, scale_bytes));
    }

    if (dynamic_offset_bytes.empty()) {
      dynamic_offset_bytes = term;
    } else {
      auto sum = new_temp();
      ir.push_back(IR::Binary::create(sum, dynamic_offset_bytes,
                                      BinaryOp::Add, term));
      dynamic_offset_bytes = sum;
    }
  }

  if (dynamic_offset_bytes.empty()) {
    if (const_offset_bytes == 0) {
      if (addr_place != base) {
        ir.push_back(IR::Assign::create(addr_place, base));
      }
    } else {
      ir.push_back(IR::BinaryImm::create(addr_place, base,
                                         BinaryOp::Add, const_offset_bytes));
    }
    return ir;
  }

  if (const_offset_bytes != 0) {
    auto with_const = new_temp();
    ir.push_back(IR::BinaryImm::create(with_const, dynamic_offset_bytes,
                                       BinaryOp::Add, const_offset_bytes));
    dynamic_offset_bytes = with_const;
  }

  ir.push_back(IR::Binary::create(addr_place, base,
                                  BinaryOp::Add, dynamic_offset_bytes));
  return ir;
}

IR::Code IRTranslator::translateFuncDef(AST::FuncDefPtr node) {
  if (ir_debug_enabled()) {
    std::cerr << "\n========== FUNCTION "
              << node->name << " ==========\n";
  }
  IR::Code ir;

  ir.push_back(IR::Function::create(node->name));

  for (auto& param : node->params) {
    std::string pname = ir_name(param->symbol, param->ident);
    ir.push_back(IR::Param::create(pname));
  }

  // Do not preload constants/global addresses at function entry.
  // Recursive tree functions often return early; preloading hurts timeout.
  const_cache.clear();
  global_addr_cache.clear();

  auto block_ir = translate(node->block);
  append_code(ir, block_ir);

  if (node->return_btype == BasicType::Void &&
      !ends_with_return_or_goto(ir)) {
    ir.push_back(IR::Return::create());
  }

  return ir;
}

IR::Code IRTranslator::translateBlock(AST::BlockPtr node) {
  IR::Code ir;

  for (auto& stmt : node->stmts) {
    auto stmt_ir = translate(stmt);
    append_code(ir, stmt_ir);

    if (!ir.empty() && std::dynamic_pointer_cast<IR::Return>(ir.back())) {
      break;
    }
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
  IRDebugScope dbg("translateAssignStmt", node->to_string());
  std::cerr << "[ASSIGN LHS] " << node->lval->to_string() << "\n";
  IR::Code ir;

  std::string lhs_name = ir_name(node->lval->symbol, node->lval->ident);
  bool is_global = global_symbols.count(lhs_name) > 0;
  bool lhs_is_array = is_array_symbol(node->lval->symbol);

  if (node->lval->indices.empty() && !is_global && !lhs_is_array) {
    std::string rhs_name;
    auto rhs_ir = translateValueName(node->exp, rhs_name);
    append_code(ir, rhs_ir);

    if (rhs_name != lhs_name) {
      ir.push_back(IR::Assign::create(lhs_name, rhs_name));
    }
    return ir;
  }

  std::string rhs_name;
  auto rhs_ir = translateValueName(node->exp, rhs_name);
  append_code(ir, rhs_ir);

  auto addr = new_temp();
  auto addr_ir = translateLValAddr(node->lval, addr);
  append_code(ir, addr_ir);

  ir.push_back(IR::Store::create(addr, rhs_name));
  return ir;
}

IR::Code IRTranslator::translateReturnStmt(AST::ReturnStmtPtr node) {
  IRDebugScope dbg("translateReturnStmt", node->to_string());
  IR::Code ir;

  if (isInlining()) {
    auto [ret_place, ret_label] = inline_return_stack.back();

    if (node->exp && !ret_place.empty()) {
      std::string ret_name;
      auto ret_ir = translateValueName(node->exp, ret_name);
      append_code(ir, ret_ir);

      if (ret_name != ret_place) {
        ir.push_back(IR::Assign::create(ret_place, ret_name));
      }
    }

    ir.push_back(IR::Goto::create(ret_label));
    return ir;
  }

  if (node->exp) {
    std::string ret_name;
    auto ret_ir = translateValueName(node->exp, ret_name);
    append_code(ir, ret_ir);
    ir.push_back(IR::Return::create(ret_name));
  } else {
    ir.push_back(IR::Return::create());
  }

  return ir;
}

IR::Code IRTranslator::translateLVal(AST::LValPtr node,
                                     const std::string& place) {
  IRDebugScope dbg("translateLVal", node->to_string());
  std::cerr << "[LVAL] " << node->ident
            << ", indices=" << node->indices.size() << "\n";
  IR::Code ir;

  std::string name = ir_name(node->symbol, node->ident);
  bool is_global = global_symbols.count(name) > 0;
  bool is_array = is_array_symbol(node->symbol);

  // local scalar: directly read variable
  if (node->indices.empty() && !is_global && !is_array) {
    if (!place.empty()) ir.push_back(IR::Assign::create(place, name));
    return ir;
  }

  // local array name: address
  if (node->indices.empty() && !is_global && is_array) {
    if (!place.empty()) ir.push_back(IR::Assign::create(place, name));
    return ir;
  }

  // global scalar: load from &name
  if (node->indices.empty() && is_global && !is_array) {
    auto addr = new_temp();
    ir.push_back(IR::LoadAddr::create(addr, name));
    if (!place.empty()) {
      ir.push_back(IR::Deref::create(place, addr));
    }
    return ir;
  }

  // global array name: address
  if (node->indices.empty() && is_global && is_array) {
    if (!place.empty()) {
      ir.push_back(IR::LoadAddr::create(place, name));
    }
    return ir;
  }

  // global array name: address
  if (node->indices.empty() && is_global && is_array) {
    if (!place.empty()) ir.push_back(IR::LoadAddr::create(place, name));
    return ir;
  }

  auto addr = new_temp();
  auto addr_ir = translateLValAddr(node, addr);
  append_code(ir, addr_ir);

  auto dims = get_array_dims_from_symbol(node->symbol);

  // partial array indexing returns address, e.g. a[2] for int a[4][2]
  if (is_array && (int)node->indices.size() < (int)dims.size()) {
    if (!place.empty()) ir.push_back(IR::Assign::create(place, addr));
    return ir;
  }

  if (!place.empty()) ir.push_back(IR::Deref::create(place, addr));
  return ir;
}

IR::Code IRTranslator::translateBinaryExp(AST::BinaryExpPtr node,
                                          const std::string& place) {
  IRDebugScope dbg("translateBinaryExp", node->to_string());
  std::cerr << "[BINARY OP] " << op_to_string(node->op) << "\n";
  IR::Code ir;

  // Boolean / relational expression used as value:
  // x = (a < b), x = (a && b), return a || b
  if (node->op == BinaryOp::LAnd || node->op == BinaryOp::LOr ||
      is_relop(node->op)) {
    return translateBoolValue(node, place);
  }

  // const op const
  int const_value;
  if (eval_const_int(node, const_value)) {
    if (!place.empty()) {
      ir.push_back(IR::LoadImm::create(place, const_value));
    }
    dump_ir_code("translateBinaryExp", ir);return ir;
  }

  // x op constant
  if (node->op == BinaryOp::Add || node->op == BinaryOp::Sub ||
      node->op == BinaryOp::Mul || node->op == BinaryOp::Div ||
      node->op == BinaryOp::Mod) {
    if (auto rc = std::dynamic_pointer_cast<AST::IntConst>(node->right)) {
      std::string left_name;
      auto left_ir = translateValueName(node->left, left_name);
      append_code(ir, left_ir);

      if (!place.empty()) {
        ir.push_back(IR::BinaryImm::create(place, left_name,
                                           node->op, rc->value));
      }
      return ir;
    }
  }

  std::string left_name;
  std::string right_name;

  auto left_ir = translateValueName(node->left, left_name);
  auto right_ir = translateValueName(node->right, right_name);

  append_code(ir, left_ir);
  append_code(ir, right_ir);

  if (!place.empty()) {
    ir.push_back(IR::Binary::create(place, left_name, node->op, right_name));
  }

  return ir;
}

IR::Code IRTranslator::translateUnaryExp(AST::UnaryExpPtr node,
                                         const std::string& place) {
  IRDebugScope dbg("translateUnaryExp", node->to_string());
  IR::Code ir;

  std::string exp_name;
  auto exp_ir = translateValueName(node->exp, exp_name);
  append_code(ir, exp_ir);

  if (place.empty()) return ir;

  if (node->op == UnaryOp::Pos) {
    if (place != exp_name) {
      ir.push_back(IR::Assign::create(place, exp_name));
    }
    return ir;
  }

  if (node->op == UnaryOp::Neg) {
    ir.push_back(IR::Unary::create(place, node->op, exp_name));
    return ir;
  }

  if (node->op == UnaryOp::Not) {
    auto zero = new_temp();
    auto true_label = new_label();
    auto false_label = new_label();
    auto end_label = new_label();

    ir.push_back(IR::LoadImm::create(zero, 0));
    ir.push_back(IR::If::create(exp_name, BinaryOp::Eq, zero, true_label));
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
  IRDebugScope dbg("translateFuncCall", node->to_string());
  std::cerr << "[CALL] " << node->name
            << ", args=" << node->args.size() << "\n";
  IR::Code ir;
  std::vector<std::string> arg_places;

  for (auto& arg : node->args) {
    std::string arg_name;
    auto arg_ir = translateValueName(arg, arg_name);
    append_code(ir, arg_ir);
    arg_places.push_back(arg_name);
  }

  bool inlined = false;
  auto inline_ir = tryInlineFuncCall(node, place, arg_places, inlined);
  if (inlined) {
    append_code(ir, inline_ir);
    return ir;
  }

  for (int i = 0; i < (int)arg_places.size(); i++) {
    ir.push_back(IR::Arg::create(arg_places[i], node->name, i));
  }

  if (!place.empty()) {
    ir.push_back(IR::Call::create(place, node->name));
  } else {
    ir.push_back(IR::Call::create(node->name));
  }

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

  if (ir_debug_enabled()) {
    std::cerr << "\n[TRANSLATE COND] " 
              << (node ? node->to_string() : "null") << "\n";
    std::cerr << "  true_label=" << true_label
              << ", false_label=" << false_label << "\n";
  }

  int const_value;
  if (eval_const_int(node, const_value)) {
    if (ir_debug_enabled()) {
      std::cerr << "  [COND KIND] constant value = "
                << const_value << "\n";
    }

    ir.push_back(IR::Goto::create(const_value != 0 ? true_label : false_label));
    dump_ir_code("translateCond", ir);
    return ir;
  }

  if (auto u = std::dynamic_pointer_cast<AST::UnaryExp>(node)) {
    if (u->op == UnaryOp::Not) {
      if (ir_debug_enabled()) {
        std::cerr << "  [COND KIND] unary NOT, swap true/false\n";
      }

      auto inner_ir = translateCond(u->exp, false_label, true_label);
      append_code(ir, inner_ir);
      dump_ir_code("translateCond", ir);
      return ir;
    }
  }

  if (auto bin = std::dynamic_pointer_cast<AST::BinaryExp>(node)) {
    if (ir_debug_enabled()) {
      std::cerr << "  [COND BINARY OP] "
                << op_to_string(bin->op) << "\n";
      std::cerr << "  [COND LEFT] "
                << (bin->left ? bin->left->to_string() : "null") << "\n";
      std::cerr << "  [COND RIGHT] "
                << (bin->right ? bin->right->to_string() : "null") << "\n";
    }

    if (bin->op == BinaryOp::LAnd) {
      if (ir_debug_enabled()) {
        std::cerr << "  [COND KIND] logical AND short-circuit\n";
      }

      auto mid_label = new_label();

      auto left_ir = translateCond(bin->left, mid_label, false_label);
      append_code(ir, left_ir);

      ir.push_back(IR::Label::create(mid_label));

      auto right_ir = translateCond(bin->right, true_label, false_label);
      append_code(ir, right_ir);

      dump_ir_code("translateCond", ir);
      return ir;
    }

    if (bin->op == BinaryOp::LOr) {
      if (ir_debug_enabled()) {
        std::cerr << "  [COND KIND] logical OR short-circuit\n";
      }

      auto mid_label = new_label();

      auto left_ir = translateCond(bin->left, true_label, mid_label);
      append_code(ir, left_ir);

      ir.push_back(IR::Label::create(mid_label));

      auto right_ir = translateCond(bin->right, true_label, false_label);
      append_code(ir, right_ir);

      dump_ir_code("translateCond", ir);
      return ir;
    }

    if (is_relop(bin->op)) {
      if (ir_debug_enabled()) {
        std::cerr << "  [COND KIND] relational judgment\n";
      }

      std::string left_name;
      std::string right_name;

      auto left_ir = translateValueName(bin->left, left_name);
      auto right_ir = translateValueName(bin->right, right_name);

      append_code(ir, left_ir);
      append_code(ir, right_ir);

      if (ir_debug_enabled()) {
        std::cerr << "  [COND VALUE LEFT] " << left_name << "\n";
        std::cerr << "  [COND VALUE RIGHT] " << right_name << "\n";
      }

      ir.push_back(IR::If::create(left_name, bin->op, right_name, true_label));
      ir.push_back(IR::Goto::create(false_label));

      dump_ir_code("translateCond", ir);
      return ir;
    }
  }

  if (ir_debug_enabled()) {
    std::cerr << "  [COND KIND] value/expression compared with zero\n";
  }

  std::string value_name;
  auto value_ir = translateValueName(node, value_name);
  append_code(ir, value_ir);

  auto zero = new_temp();
  ir.push_back(IR::LoadImm::create(zero, 0));

  if (ir_debug_enabled()) {
    std::cerr << "  [COND VALUE] " << value_name << "\n";
    std::cerr << "  [COND ZERO] " << zero << "\n";
  }

  ir.push_back(IR::If::create(value_name, BinaryOp::Ne, zero, true_label));
  ir.push_back(IR::Goto::create(false_label));

  dump_ir_code("translateCond", ir);
  return ir;
}

IR::Code IRTranslator::translateIfStmt(AST::IfStmtPtr node) {
  IRDebugScope dbg("translateIfStmt", node->to_string());
  std::cerr << "[IF COND AST] " << node->cond->to_string() << "\n";
  IR::Code ir;
  if (ir_debug_enabled()) {
    std::cerr << "\n[IF STMT] " << node->to_string() << "\n";
    std::cerr << "[IF COND AST] " << node->cond->to_string() << "\n";
  }

  auto true_label = new_label();
  auto end_label = new_label();

  // No else: jump false directly to end.
  if (!node->else_stmt) {
    auto cond_ir = translateCond(node->cond, true_label, end_label);
    append_code(ir, cond_ir);

    ir.push_back(IR::Label::create(true_label));

    auto then_ir = translate(node->then_stmt);
    bool then_ends = ends_with_return_or_goto(then_ir);
    append_code(ir, then_ir);

    if (!then_ends) {
      ir.push_back(IR::Goto::create(end_label));
    }

    ir.push_back(IR::Label::create(end_label));
    dump_ir_code("translateIfStmt", ir);return ir;
  }

  // Has else.
  auto false_label = new_label();

  auto cond_ir = translateCond(node->cond, true_label, false_label);
  append_code(ir, cond_ir);

  ir.push_back(IR::Label::create(true_label));

  auto then_ir = translate(node->then_stmt);
  bool then_ends = ends_with_return_or_goto(then_ir);
  append_code(ir, then_ir);

  if (!then_ends) {
    ir.push_back(IR::Goto::create(end_label));
  }

  ir.push_back(IR::Label::create(false_label));

  auto else_ir = translate(node->else_stmt);
  bool else_ends = ends_with_return_or_goto(else_ir);
  append_code(ir, else_ir);

  if (!else_ends) {
    ir.push_back(IR::Goto::create(end_label));
  }

  ir.push_back(IR::Label::create(end_label));
  return ir;
}

IR::Code IRTranslator::translateWhileStmt(AST::WhileStmtPtr node) {
  IRDebugScope dbg("translateWhileStmt", node->to_string());
  if (ir_debug_enabled()) {
    std::cerr << "\n[WHILE STMT] " << node->to_string() << "\n";
    std::cerr << "[WHILE COND AST] " << node->cond->to_string() << "\n";
  }
  std::cerr << "[WHILE COND AST] " << node->cond->to_string() << "\n";
  IR::Code ir;

  auto cond_label = new_label();
  auto body_label = new_label();
  auto end_label = new_label();

  ir.push_back(IR::Label::create(cond_label));

  auto cond_ir = translateCond(node->cond, body_label, end_label);
  append_code(ir, cond_ir);

  ir.push_back(IR::Label::create(body_label));

  auto body_ir = translate(node->body);
  bool body_ends = ends_with_return_or_goto(body_ir);
  append_code(ir, body_ir);

  if (!body_ends) {
    ir.push_back(IR::Goto::create(cond_label));
  }

  ir.push_back(IR::Label::create(end_label));
  dump_ir_code("translateWhileStmt", ir);return ir;
}

IR::Code IRTranslator::tryInlineFuncCall(AST::FuncCallPtr node,
                                         const std::string& place,
                                         const std::vector<std::string>& arg_places,
                                         bool& inlined) {
  IR::Code ir;
  inlined = false;
  return ir;
}

IR::Code IRTranslator::translateBoolValue(AST::NodePtr node,
                                          const std::string& place) {
  IR::Code ir;

  auto true_label = new_label();
  auto false_label = new_label();
  auto end_label = new_label();

  auto cond_ir = translateCond(node, true_label, false_label);
  append_code(ir, cond_ir);

  ir.push_back(IR::Label::create(true_label));
  if (!place.empty()) {
    ir.push_back(IR::LoadImm::create(place, 1));
  }
  ir.push_back(IR::Goto::create(end_label));

  ir.push_back(IR::Label::create(false_label));
  if (!place.empty()) {
    ir.push_back(IR::LoadImm::create(place, 0));
  }
  ir.push_back(IR::Goto::create(end_label));

  ir.push_back(IR::Label::create(end_label));
  return ir;
}