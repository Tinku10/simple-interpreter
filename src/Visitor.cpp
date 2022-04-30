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

void BinaryNode::accept(Visitor& v) {
  v.visit(*this);
}

void UnaryNode::accept(Visitor& v) {
  v.visit(*this);
}

void AssignNode::accept(Visitor& v) {
  v.visit(*this);
}

void VarNode::accept(Visitor& v) {
  v.visit(*this);
}

void LiteralNode::accept(Visitor& v) {
  v.visit(*this);
}

void CompoundNode::accept(Visitor& v) {
  v.visit(*this);
}

void NoOpNode::accept(Visitor& v) {
  v.visit(*this);
}

void Visitor::visit(BinaryNode& node) {
  node.left->accept(*this);
  int left = value;
  node.right->accept(*this);
  int right = value;

  switch (node.token.type) {
    case TokenType::PLUS: value = left + right; break;
    case TokenType::MINUS: value = left - right; break;
    case TokenType::MULTIPLY: value = left * right; break;
    case TokenType::DIVIDE: value = left / right; break;
    default: throw std::invalid_argument("Invalid binary operator");
  }
}

void Visitor::visit(UnaryNode& node) {
  node.node->accept(*this);
  switch (node.token.type) {
    case TokenType::PLUS: value = value; break;
    case TokenType::MINUS: value = -value; break;
    default: throw std::invalid_argument("Invalid unary operator");
  }
}

void Visitor::visit(VarNode& node) {
  name = node.token.value;
  if (callstack.cache.count(name))
    value = callstack.cache.at(name);
  else callstack.cache[name] = 0;

  /* throw std::invalid_argument("Undeclared identifier"); */
}

void Visitor::visit(AssignNode& node) {
  node.left->accept(*this);
  std::string left = name;
  node.right->accept(*this);
  int right = value;
  callstack.cache[left] = right;

  std::cout << left << " = " << right << "\n";
}

void Visitor::visit(LiteralNode& node) {
  // int type only for now
  value = stoi(node.token.value);
}

void Visitor::visit(CompoundNode& node) {
  for (auto child : node.children) {
    child->accept(*this);
  }
}

void Visitor::visit(NoOpNode& node) {
  return;
}