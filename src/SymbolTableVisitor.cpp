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

void TypeNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
}

void NoOpNode::accept(SymbolTableVisitor& v) {
  v.visit(*this);
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
  symbols.at(name);
}

void SymbolTableVisitor::visit(AssignNode& node) {
  node.left->accept(*this);

  std::string var = name;

  symbols.at(var);

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
  node.child->accept(*this);
}

void SymbolTableVisitor::visit(BlockNode& node) {
  for (auto& child : node.declarations) {
    child->accept(*this);
  }

  node.compound_statement->accept(*this);
}

void SymbolTableVisitor::visit(VarDeclNode& node) {
  std::string name = std::dynamic_pointer_cast<VarNode>(node.var)->token.value;
  std::string type = std::dynamic_pointer_cast<TypeNode>(node.type)->token.value;

  symbols.add(
      std::make_shared<VarTypeSymbol>(VarTypeSymbol(name, symbols.at(type))));
}

void SymbolTableVisitor::visit(ProcedureDeclNode& node) {
  return;
}

void SymbolTableVisitor::visit(TypeNode& node) {
  return;
}

void SymbolTableVisitor::visit(NoOpNode& node) {
  return;
}
