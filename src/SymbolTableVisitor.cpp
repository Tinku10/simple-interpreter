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
    std::invalid_argument("Undeclared identifier");
  }
}

void SymbolTableVisitor::visit(AssignNode& node) {
  node.left->accept(*this);

  std::string var = name;

  if (current_scope->at(var) == nullptr) {
    std::invalid_argument("Undeclared identifier");
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

  current_scope
      = std::make_shared<ScopedSymbolTable>(ScopedSymbolTable("GLOBAL", 1, current_scope));

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

  std::cout << name << " " << type << "\n";
  if (current_scope->at(name, 1)) {
    throw std::invalid_argument("Duplicated identifier found");
  }

  current_scope->add(
      std::make_shared<VarTypeSymbol>(VarTypeSymbol(name, symbol->symbol)));
  std::cout << *current_scope << "\n";
}

void SymbolTableVisitor::visit(ProcedureDeclNode& node) {
  std::cout << "Enter Procedure scope\n";

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

  current_scope->parent_scope->add(symbol);

  node.block->accept(*this);

  current_scope = current_scope->parent_scope;

  std::cout << "Leave Procedure scope\n";
}

void SymbolTableVisitor::visit(ParamsNode& node) {
}

void SymbolTableVisitor::visit(TypeNode& node) {
  return;
}

void SymbolTableVisitor::visit(NoOpNode& node) {
  return;
}
