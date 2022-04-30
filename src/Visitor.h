#include <vector>

#include "Token.h"
#include "CallStack.h"

class Visitor;

class Node {
 public:
  Token token;
  virtual void accept(Visitor& v) = 0;
};

class BinaryNode : public Node {
 public:
  std::shared_ptr<Node> left;
  Token token;
  std::shared_ptr<Node> right;

  BinaryNode(std::shared_ptr<Node> left,
             Token token,
             std::shared_ptr<Node> right);

  void accept(Visitor& v) override;
};

class UnaryNode : public Node {
 public:
  std::shared_ptr<Node> node;
  Token token;

  UnaryNode(std::shared_ptr<Node> node, Token token);
  void accept(Visitor& v) override;
};

class AssignNode : public Node {
 public:
  std::shared_ptr<Node> left;
  Token token;
  std::shared_ptr<Node> right;

  AssignNode(std::shared_ptr<Node> left,
             Token token,
             std::shared_ptr<Node> right);
  void accept(Visitor& v) override;
};

class VarNode : public Node {
 public:
  Token token;

  VarNode(Token token);
  void accept(Visitor& v) override;
};

class LiteralNode : public Node {
 public:
  Token token;

  LiteralNode(Token token);
  void accept(Visitor& v) override;
};

class CompoundNode : public Node {
 public:
  /* Token token; */
  std::vector<std::shared_ptr<Node>> children;

  CompoundNode(std::vector<std::shared_ptr<Node>> children);
  void accept(Visitor& v) override;
};

class NoOpNode : public Node {
public:
  void accept(Visitor& v) override;
};

class Visitor {
 public:
  int value;
  std::string name;

  CallStack callstack;
  void visit(BinaryNode& node);
  void visit(UnaryNode& node);
  void visit(AssignNode& node);
  void visit(LiteralNode& node);
  void visit(VarNode& node);
  void visit(CompoundNode& node);
  void visit(NoOpNode& node);
};
