#include <unordered_map>

#include "Visitor.h"

void BinaryNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

void UnaryNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

void AssignNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

void VarNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

void LiteralNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

void CompoundNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

void ProgramNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

void BlockNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

void VarDeclNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

void ProcedureDeclNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

void ProcedureCallNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

void ParamsNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

void TypeNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

void NoOpNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

SymbolTableVisitor::SymbolTableVisitor()
    : current_scope(
        std::make_shared<ScopedSymbolTable>(ScopedSymbolTable("BUILTIN", 0))) {
}

void SymbolTableVisitor::visit(BinaryNode& node) {
  node.left->accept(*this);
  node.right->accept(*this);
}

void SymbolTableVisitor::visit(UnaryNode& node) {
  node.node->accept(*this);
}

void SymbolTableVisitor::visit(VarNode& node) {
  name = node.token.value;
  if (current_scope->at(name) == nullptr) {
    throw error(ErrorCode::IDENTIFIER_NOT_FOUND, node.token);
    /* std::invalid_argument("Undeclared identifier"); */
  }
}

void SymbolTableVisitor::visit(AssignNode& node) {
  node.left->accept(*this);

  std::string var = name;

  if (current_scope->at(var) == nullptr) {
    throw error(ErrorCode::IDENTIFIER_NOT_FOUND, node.token);
    /* std::invalid_argument("Undeclared identifier"); */
  }

  node.right->accept(*this);
}

void SymbolTableVisitor::visit(LiteralNode& node) {
  return;
}

void SymbolTableVisitor::visit(CompoundNode& node) {
  for (auto& child : node.children) {
    child->accept(*this);
  }
}

void SymbolTableVisitor::visit(ProgramNode& node) {
  std::cout << "Enter GLOBAL scope\n";

  current_scope = std::make_shared<ScopedSymbolTable>(
      ScopedSymbolTable("GLOBAL", 1, current_scope));

  node.child->accept(*this);

  current_scope = current_scope->parent_scope;

  std::cout << "Leave GLOBAL scope\n";
}

void SymbolTableVisitor::visit(BlockNode& node) {
  for (auto& child : node.declarations) {
    child->accept(*this);
  }

  node.compound_statement->accept(*this);
}

void SymbolTableVisitor::visit(VarDeclNode& node) {
  std::string name = std::dynamic_pointer_cast<VarNode>(node.var)->token.value;
  std::string type
      = std::dynamic_pointer_cast<TypeNode>(node.type)->token.value;

  std::shared_ptr<SymbolWithScope> symbol = current_scope->at(type);

  if (current_scope->at(name, 0, 1)) {
    throw error(ErrorCode::DUPLICATE_IDENTIFIER,
                std::dynamic_pointer_cast<VarNode>(node.var)->token);
    /* throw std::invalid_argument("Duplicated identifier found"); */
  }

  current_scope->add(
      std::make_shared<VarTypeSymbol>(VarTypeSymbol(name, symbol->symbol)));
}

void SymbolTableVisitor::visit(ProcedureDeclNode& node) {
  std::cout << "Enter " << node.name << " scope\n";

  current_scope = std::make_shared<ScopedSymbolTable>(ScopedSymbolTable(
      std::move(node.name), current_scope->scope_level + 1, current_scope));

  std::vector<std::shared_ptr<Symbol>> list;

  for (auto& child : node.params) {
    std::shared_ptr<ParamsNode> param
        = std::static_pointer_cast<ParamsNode>(child);
    std::string& name
        = std::static_pointer_cast<VarNode>(param->var)->token.value;
    std::string& type
        = std::static_pointer_cast<VarNode>(param->type)->token.value;

    std::shared_ptr<VarTypeSymbol> var = std::make_shared<VarTypeSymbol>(
        VarTypeSymbol(name, current_scope->at(type)->symbol));

    current_scope->add(var);
    list.push_back(var);
    child->accept(*this);

  }

  std::shared_ptr<ProcedureSymbol> symbol
      = std::make_shared<ProcedureSymbol>(ProcedureSymbol(node.name, list));

  symbol->block = node.block;

  current_scope->parent_scope->add(symbol);

  node.block->accept(*this);

  current_scope = current_scope->parent_scope;

  std::cout << "Leave " << node.name << " scope\n";
}

void SymbolTableVisitor::visit(ProcedureCallNode& node) {
  std::shared_ptr<SymbolWithScope> symbol
      = current_scope->at(node.name, node.params.size());

  if (!symbol) {
    throw error(ErrorCode::IDENTIFIER_NOT_FOUND, node.token);
  }

  node.procedure_symbol
      = std::dynamic_pointer_cast<ProcedureSymbol>(symbol->symbol);

  for (auto& child : node.params) {
    child->accept(*this);
  }
}

void SymbolTableVisitor::visit(ParamsNode& node) {
}

void SymbolTableVisitor::visit(TypeNode& node) {
  return;
}

void SymbolTableVisitor::visit(NoOpNode& node) {
  return;
}

Exception SymbolTableVisitor::error(ErrorCode code, Token& token) {
  std::string msg = "";
  switch (code) {
    case ErrorCode::DUPLICATE_IDENTIFIER: msg = "duplicate identifier\n"; break;
    case ErrorCode::IDENTIFIER_NOT_FOUND:
      msg = "undeclared identifier\n";
      break;
    default: msg = "unknown error\n";
  }

  msg += "symantic error at line " + std::to_string(token.line) + " column "
         + std::to_string(token.start);
  throw Exception(code, msg);
}
