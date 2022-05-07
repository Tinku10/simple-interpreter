#include <unordered_map>

#include "Visitor.h"

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

CompoundNode::CompoundNode(std::vector<std::shared_ptr<Node>>& children)
    : children(children) {
}

ProgramNode::ProgramNode(std::shared_ptr<Node> var, std::shared_ptr<Node> child)
    : var(var), child(child) {
}

BlockNode::BlockNode(std::vector<std::shared_ptr<Node>>& declarations,
                     std::shared_ptr<Node> compound_statement)
    : declarations(declarations), compound_statement(compound_statement) {
}

VarDeclNode::VarDeclNode(std::shared_ptr<Node> var, std::shared_ptr<Node> type)
    : var(var), type(type) {
}

ProcedureDeclNode::ProcedureDeclNode(std::string name,
                                     std::vector<std::shared_ptr<Node>>& params,
                                     std::shared_ptr<Node> block)
    : name(name), params(params), block(block) {
}

ProcedureCallNode::ProcedureCallNode(Token& token,
                                     std::string& name,
                                     std::vector<std::shared_ptr<Node>>& params)
    : token(token), name(name), params(params) {
}

ParamsNode::ParamsNode(std::shared_ptr<Node> var, std::shared_ptr<Node> type)
    : var(var), type(type) {
}

TypeNode::TypeNode(Token token) : token(token) {
}

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

void ProcedureDeclNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void ProcedureCallNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void ParamsNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void TypeNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

void NoOpNode::accept(NodeVisitor& v) {
  v.visit(*this);
}

NodeVisitor::NodeVisitor() : callstack(CallStack()) {
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
  value = callstack.top()->at(name);

  /* throw std::invalid_argument("Undeclared identifier"); */
}

void NodeVisitor::visit(AssignNode& node) {
  std::string left = std::static_pointer_cast<VarNode>(node.left)->token.value;

  node.right->accept(*this);
  int right = value;

  callstack.top()->add(left, right);
  /* callstack.cache[left] = right; */

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
  callstack.push(std::move(std::make_unique<ActivationRecords>(
      ActivationRecords("PROGRAM", ActivationRecordType::PROGRAM, 1))));

  node.child->accept(*this);

  callstack.pop();
}

void NodeVisitor::visit(BlockNode& node) {
  for (auto& child : node.declarations) {
    child->accept(*this);
  }

  node.compound_statement->accept(*this);
}

void NodeVisitor::visit(VarDeclNode& node) {
  return;
}

void NodeVisitor::visit(ProcedureDeclNode& node) {
  return;
}

void NodeVisitor::visit(ProcedureCallNode& node) {
  for (auto& child : node.params) {
    child->accept(*this);
  }
}

void NodeVisitor::visit(ParamsNode& node) {
  return;
}

void NodeVisitor::visit(TypeNode& node) {
  return;
}

void NodeVisitor::visit(NoOpNode& node) {
  return;
}
