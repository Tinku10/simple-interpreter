#include "Visitor.h"

#include <unordered_map>

BinaryNode::BinaryNode(std::shared_ptr<Node> left,
                       Token token,
                       std::shared_ptr<Node> right)
    : left(left), token(token), right(right) {
}

UnaryNode::UnaryNode(std::shared_ptr<Node> node, Token token)
    : node(node), token(token) {
}

AssignNode::AssignNode(std::shared_ptr<Node> left,
                       Token token,
                       std::shared_ptr<Node> right)
    : left(left), token(token), right(right) {
}

VarNode::VarNode(Token token) : token(token) {
}

LiteralNode::LiteralNode(Token token) : token(token) {
}

CompoundNode::CompoundNode(std::vector<std::shared_ptr<Node>> children)
    : children(children) {
}

ProgramNode::ProgramNode(std::shared_ptr<Node> child) : child(child) {
}

BlockNode::BlockNode(std::vector<std::shared_ptr<Node>> declarations,
                     std::shared_ptr<Node> compound_statement)
    : declarations(declarations), compound_statement(compound_statement) {
}

VarDeclNode::VarDeclNode(std::shared_ptr<Node> var, std::shared_ptr<Node> type)
  : var(var), type(type) {}

TypeNode::TypeNode(Token token) : token(token) {}

void BinaryNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void UnaryNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void AssignNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void VarNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void LiteralNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void CompoundNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void ProgramNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void BlockNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void VarDeclNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void TypeNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void NoOpNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void NodeVisitor::visit(BinaryNode& node) {
  node.left->accept(*this);
  int left = value;
  node.right->accept(*this);
  int right = value;

  switch (node.token.type) {
    case TokenType::PLUS: value = left + right; break;
    case TokenType::MINUS: value = left - right; break;
    case TokenType::MULTIPLY: value = left * right; break;
    case TokenType::INT_DIV: value = left / right; break;
    case TokenType::FLOAT_DIV: value = left / (float)right; break;
    default: throw std::invalid_argument("Invalid binary operator");
  }

}

void NodeVisitor::visit(UnaryNode& node) {
  node.node->accept(*this);
  switch (node.token.type) {
    case TokenType::PLUS: value = value; break;
    case TokenType::MINUS: value = -value; break;
    default: throw std::invalid_argument("Invalid unary operator");
  }
}

void NodeVisitor::visit(VarNode& node) {
  name = node.token.value;
  if (callstack.cache.count(name))
    value = callstack.cache.at(name);
  else
    callstack.cache[name] = 0;

  /* throw std::invalid_argument("Undeclared identifier"); */
}

void NodeVisitor::visit(AssignNode& node) {
  node.left->accept(*this);
  std::string left = name;
  node.right->accept(*this);
  int right = value;
  callstack.cache[left] = right;

  std::cout << left << " = " << right << "\n";
}

void NodeVisitor::visit(LiteralNode& node) {
  // int type only for now
  value = stoi(node.token.value);
}

void NodeVisitor::visit(CompoundNode& node) {
  for (auto& child : node.children) {
    child->accept(*this);
  }
}

void NodeVisitor::visit(ProgramNode& node) {
  node.child->accept(*this);
}

void NodeVisitor::visit(BlockNode& node) {
  for(auto& child: node.declarations) {
    child->accept(*this);
  }

  node.compound_statement->accept(*this);
}

void NodeVisitor::visit(VarDeclNode& node) {
  return;
}

void NodeVisitor::visit(TypeNode& node) {
  return;
}

void NodeVisitor::visit(NoOpNode& node) {
  return;
}
