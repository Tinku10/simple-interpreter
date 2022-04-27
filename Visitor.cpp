#include "Visitor.h"
#include<unordered_map>

template <typename T>
std::unordered_map<std::string, T> memory;
std::unordered_map<std::string, TokenType> reserved;

template <>
int BinaryNode<int>::accept(Visitor& v) {
  return v.visit(*this);
}

template <>
int UnaryNode<int>::accept(Visitor& v) {
  return v.visit(*this);
}

template <>
void AssignNode<void>::accept(Visitor& v) {
  v.visit(*this);
}

template <>
int VarNode<int>::accept(Visitor& v) {
  return v.visit(*this);
}

template <>
std::string LiteralNode<std::string>::accept(Visitor& v) {
  return v.visit(*this);
}

template <>
int LiteralNode<int>::accept(Visitor& v) {
  return v.visit(*this);
}

int Visitor::visit(BinaryNode<int>& node) {
  int left = node.left->accept(*this);
  int right = node.right->accept(*this);

  switch(node.token->type) {
    case TokenType::PLUS: return left + right;
    case TokenType::MINUS: return left - right;
    case TokenType::MULTIPLY: return left * right;
    case TokenType::DIVIDE: return left / right;
    default:
      return 0;
      throw std::invalid_argument("Invalid binary operator");
  }
}

int Visitor::visit(UnaryNode<int>& node) {
  switch(node.token->type) {
    case TokenType::PLUS: return node.node->accept(*this);
    case TokenType::MINUS: return -node.node->accept(*this);
    default:
      throw std::invalid_argument("Invalid unary operator");
  }
}

int Visitor::visit(VarNode<int>& node) {
  if(memory<int>.count(node.token->value)) return memory<int>.count(node.token->value);

  throw std::invalid_argument("Undeclared identifier");
}

void Visitor::visit(AssignNode<void>& node) {
  memory[node.left.token->value] = node.right->accept(*this);
}

std::string Visitor::visit(LiteralNode<std::string>& node) {
  return node.token->value;
}
