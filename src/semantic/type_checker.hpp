#ifndef SEMANTIC_TYPE_CHECKER_HPP
#define SEMANTIC_TYPE_CHECKER_HPP

#include <memory>

#include "ast/tree.hpp"
#include "symbol_table.hpp"

class TypeChecker {
 public:
  TypeChecker();

  TypePtr check(AST::NodePtr node);

 private:
  int count_init_elements(AST::InitValPtr node);
  bool is_list_init(AST::InitValPtr node);
  int array_capacity(const std::vector<int>& dims);
  int validate_array_init(AST::InitValPtr init, const std::vector<int>& dims);  /// @brief The symbol table
  SymbolTable symbol_table;
  TypePtr current_return_type;


  TypePtr checkCompUnit(AST::CompUnitPtr node);
  TypePtr checkFuncDef(AST::FuncDefPtr node);
  TypePtr checkVarDecl(AST::VarDeclPtr node);
  TypePtr checkVarDef(AST::VarDefPtr node, BasicType var_type);
  TypePtr checkBlock(AST::BlockPtr node, bool new_scope = true);
  TypePtr checkAssignStmt(AST::AssignStmtPtr node);
  TypePtr checkReturnStmt(AST::ReturnStmtPtr node);
  TypePtr checkIfStmt(AST::IfStmtPtr node);
  TypePtr checkWhileStmt(AST::WhileStmtPtr node);
  TypePtr checkLVal(AST::LValPtr node);
  TypePtr checkIntConst(AST::IntConstPtr node);
  TypePtr checkFuncCall(AST::FuncCallPtr node);
  TypePtr checkUnaryExp(AST::UnaryExpPtr node);
  TypePtr checkBinaryExp(AST::BinaryExpPtr node);
  TypePtr checkInitVal(AST::InitValPtr node);
  TypePtr checkExprStmt(AST::ExprStmtPtr node);
};

#endif  // SEMANTIC_TYPE_CHECKER_HPP