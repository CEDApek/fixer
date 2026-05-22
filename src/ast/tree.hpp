#ifndef AST_TREE_HPP
#define AST_TREE_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "common.hpp"
#include "semantic/symbol_table.hpp"

extern int yylineno;

namespace AST {

class Node;
using NodePtr = std::shared_ptr<Node>;

class Node {
 public:
  int lineno;

  virtual std::vector<NodePtr> get_children() { return std::vector<NodePtr>(); }
  void print_tree(std::string prefix = "", std::string info_prefix = "");
  virtual std::string to_string() = 0;

  Node() : lineno(yylineno) {}
  virtual ~Node() = default;
};

//
class DimList;
using DimListPtr = std::shared_ptr<DimList>;
class DimList : public Node {
 public:
  std::vector<int> dims;
  DimList() = default;
  DimList(int dim) { add_dim(dim); }
  void add_dim(int dim) { dims.push_back(dim); }
  std::string to_string() override { return "DimList"; }
};
//

class IntConst;
using IntConstPtr = std::shared_ptr<IntConst>;
class IntConst : public Node {
 public:
  int value;
  IntConst(int value) : value(value) {}
  std::string to_string() override {
    return "IntConst <value: " + std::to_string(value) + ">";
  }
};

//
class InitVal;
using InitValPtr = std::shared_ptr<InitVal>;
class InitVal : public Node {
 public:
  bool is_list;
  NodePtr expr;
  std::vector<InitValPtr> elements;

  InitVal() : is_list(true), expr(nullptr) {}
  InitVal(NodePtr expr) : is_list(false), expr(expr) {}

  void add_elem(InitValPtr val) { elements.push_back(val); }

  std::string to_string() override {
    return is_list ? "InitVal <list>" : "InitVal <expr>";
  }

  std::vector<NodePtr> get_children() override {
    if (!is_list) {
      return expr ? std::vector<NodePtr>{expr} : std::vector<NodePtr>();
    }
    return std::vector<NodePtr>(elements.begin(), elements.end());
  }
};
//

class LVal;
using LValPtr = std::shared_ptr<LVal>;
class LVal : public Node {
 public:
  std::string ident;
  std::vector<NodePtr> indices;
  SymbolPtr symbol;
  LVal(std::string ident) : ident(ident) {}
  void add_index(NodePtr idx) { indices.push_back(idx); }
  std::string to_string() override { return "LVal <ident: " + ident + ">"; }
  std::vector<NodePtr> get_children() override { return indices; }
};

class UnaryExp;
using UnaryExpPtr = std::shared_ptr<UnaryExp>;
class UnaryExp : public Node {
 public:
  UnaryOp op;
  NodePtr exp;
  UnaryExp(UnaryOp op, NodePtr exp) : op(op), exp(exp) {}
  std::string to_string() override {
    return "UnaryExp <op: " + std::string(op_to_string(op)) + ">";
  }
  std::vector<NodePtr> get_children() override { return {exp}; }
};

class BinaryExp;
using BinaryExpPtr = std::shared_ptr<BinaryExp>;
class BinaryExp : public Node {
 public:
  BinaryOp op;
  NodePtr left, right;

  BinaryExp(BinaryOp op, NodePtr left, NodePtr right)
      : op(op), left(left), right(right) {}
  std::string to_string() override {
    return "BinaryExp <op: " + std::string(op_to_string(op)) + ">";
  }
  std::vector<NodePtr> get_children() override { return {left, right}; }
};

class FuncCall;
using FuncCallPtr = std::shared_ptr<FuncCall>;
class FuncCall : public Node {
 public:
  std::string name;
  std::vector<NodePtr> args;
  SymbolPtr symbol;
  FuncCall(std::string name) : name(std::move(name)) {}
  FuncCall(NodePtr exp) { add_arg(exp); }
  void add_arg(NodePtr exp) { args.push_back(exp); }
  std::string to_string() override { return "FuncCall <name: " + name + ">"; }
  std::vector<NodePtr> get_children() override { return args; }
};

class Block;
using BlockPtr = std::shared_ptr<Block>;
class Block : public Node {
 public:
  std::vector<NodePtr> stmts;
  Block() {}
  Block(NodePtr stmt) { add_stmt(stmt); }
  void add_stmt(NodePtr stmt) { stmts.push_back(stmt); }
  std::string to_string() override { return "Block"; }
  std::vector<NodePtr> get_children() override { return stmts; }
};

class AssignStmt;
using AssignStmtPtr = std::shared_ptr<AssignStmt>;
class AssignStmt : public Node {
 public:
  LValPtr lval;
  NodePtr exp;
  AssignStmt(LValPtr lval, NodePtr exp) : lval(lval), exp(exp) {}
  std::string to_string() override { return "AssignStmt"; }
  std::vector<NodePtr> get_children() override { return {lval, exp}; }
};

//
class ExprStmt;
using ExprStmtPtr = std::shared_ptr<ExprStmt>;
class ExprStmt : public Node {
 public:
  NodePtr exp;
  ExprStmt() : exp(nullptr) {}
  ExprStmt(NodePtr exp) : exp(exp) {}
  std::string to_string() override { return exp ? "ExprStmt" : "EmptyStmt"; }
  std::vector<NodePtr> get_children() override {
    return exp ? std::vector<NodePtr>{exp} : std::vector<NodePtr>();
  }
};
//

class ReturnStmt;
using ReturnStmtPtr = std::shared_ptr<ReturnStmt>;
class ReturnStmt : public Node {
 public:
  NodePtr exp;
  ReturnStmt() : exp(nullptr) {}
  ReturnStmt(NodePtr exp) : exp(exp) {}
  std::string to_string() override { return "ReturnStmt"; }
  std::vector<NodePtr> get_children() override {
    return exp ? std::vector<NodePtr>{exp} : std::vector<NodePtr>();
  }
};

//
class IfStmt;
using IfStmtPtr = std::shared_ptr<IfStmt>;
class IfStmt : public Node {
 public:
  NodePtr cond;
  NodePtr then_stmt;
  NodePtr else_stmt;

  IfStmt(NodePtr cond, NodePtr then_stmt)
      : cond(cond), then_stmt(then_stmt), else_stmt(nullptr) {}

  IfStmt(NodePtr cond, NodePtr then_stmt, NodePtr else_stmt)
      : cond(cond), then_stmt(then_stmt), else_stmt(else_stmt) {}

  std::string to_string() override { return "IfStmt"; }

  std::vector<NodePtr> get_children() override {
    if (else_stmt) return {cond, then_stmt, else_stmt};
    return {cond, then_stmt};
  }
};


class WhileStmt;
using WhileStmtPtr = std::shared_ptr<WhileStmt>;
class WhileStmt : public Node {
 public:
  NodePtr cond;
  NodePtr body;
  WhileStmt(NodePtr cond, NodePtr body) : cond(cond), body(body) {}
  std::string to_string() override { return "WhileStmt"; }
  std::vector<NodePtr> get_children() override { return {cond, body}; }
};
//

class VarDef;
using VarDefPtr = std::shared_ptr<VarDef>;
class VarDef : public Node {
 public:
  std::string ident;
  std::vector<int> dims;
  InitValPtr init;
  SymbolPtr symbol;

  VarDef(std::string ident) : ident(std::move(ident)), init(nullptr) {}
  void add_dim(int dim) { dims.push_back(dim); }
  void set_init(InitValPtr init_val) { init = init_val; }
  std::string to_string() override {
    return "VarDef <ident: " + ident + ">";
  }
  std::vector<NodePtr> get_children() override {
    return init ? std::vector<NodePtr>{init} : std::vector<NodePtr>();
  }
};

class VarDecl;
using VarDeclPtr = std::shared_ptr<VarDecl>;
class VarDecl : public Node {
 public:
  BasicType btype;
  std::vector<VarDefPtr> defs;
  VarDecl(VarDefPtr def) : btype(BasicType::Unknown) { add_def(def); }
  void add_def(VarDefPtr def) { defs.push_back(def); }
  std::string to_string() override {
    return "VarDecl <btype: " + std::string(type_to_string(btype)) + ">";
  }
  std::vector<NodePtr> get_children() override {
    return std::vector<NodePtr>(defs.begin(), defs.end());
  }
};

//
class Param;
using ParamPtr = std::shared_ptr<Param>;
class Param : public Node {
 public:
  BasicType btype;
  std::string ident;
  bool is_array;
  std::vector<int> dims_after_first;
  SymbolPtr symbol;

  Param(BasicType btype, std::string ident)
      : btype(btype), ident(std::move(ident)), is_array(false) {}

  void set_array(bool val = true) { is_array = val; }
  void add_dim(int dim) { dims_after_first.push_back(dim); }

  std::string to_string() override {
    return "Param <btype: " + std::string(type_to_string(btype)) +
           ", ident: " + ident + ">";
  }
};

class ParamList;
using ParamListPtr = std::shared_ptr<ParamList>;
class ParamList : public Node {
 public:
  std::vector<ParamPtr> params;

  ParamList(ParamPtr param) { add_param(param); }

  void add_param(ParamPtr param) { params.push_back(param); }

  std::string to_string() override { return "ParamList"; }

  std::vector<NodePtr> get_children() override {
    return std::vector<NodePtr>(params.begin(), params.end());
  }
};
//

class FuncDef;
using FuncDefPtr = std::shared_ptr<FuncDef>;
class FuncDef : public Node {
 public:
  BasicType return_btype;
  std::string name;
  std::vector<ParamPtr> params;
  BlockPtr block;
  SymbolPtr symbol;
  FuncDef(BasicType return_btype, std::string name, BlockPtr block)
      : return_btype(return_btype),
        name(std::move(name)),
        block(std::move(block)) {}

  FuncDef(BasicType return_btype, std::string name,
          std::vector<ParamPtr> params, BlockPtr block)
      : return_btype(return_btype),
        name(std::move(name)),
        params(std::move(params)),
        block(std::move(block)) {}

  std::string to_string() override {
    return "FuncDef <return_btype: " +
           std::string(type_to_string(return_btype)) + ", name: " + name + ">";
  }

  std::vector<NodePtr> get_children() override {
    std::vector<NodePtr> children(params.begin(), params.end());
    children.push_back(block);
    return children;
  }
};

class CompUnit;
using CompUnitPtr = std::shared_ptr<CompUnit>;
class CompUnit : public Node {
 public:
  std::vector<NodePtr> units;  // FuncDef or VarDecl

  CompUnit(NodePtr unit) { add_unit(unit); }

  void add_unit(NodePtr unit) { units.push_back(unit); }

  std::string to_string() override { return "CompUnit"; }

  std::vector<NodePtr> get_children() override { return units; }
};

}  // namespace AST

#endif  // AST_TREE_HPP