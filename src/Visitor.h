#include <vector>

#include "CallStack.h"
#include "SymbolTable.h"
#include "Token.h"

class NodeVisitor;
class SymbolTableVisitor;

class Node {
 public:
  Token token;
  virtual void accept(NodeVisitor& v) = 0;
  virtual void accept(SymbolTableVisitor& v) = 0;
};

class BinaryNode : public Node {
 public:
  std::shared_ptr<Node> left;
  Token token;
  std::shared_ptr<Node> right;

  BinaryNode(std::shared_ptr<Node> left,
             Token token,
             std::shared_ptr<Node> right);

  void accept(NodeVisitor& v) override;
  void accept(SymbolTableVisitor& v) override;
};

class UnaryNode : public Node {
 public:
  std::shared_ptr<Node> node;
  Token token;

  UnaryNode(std::shared_ptr<Node> node, Token token);
  void accept(NodeVisitor& v) override;
  void accept(SymbolTableVisitor& v) override;
};

class AssignNode : public Node {
 public:
  std::shared_ptr<Node> left;
  Token token;
  std::shared_ptr<Node> right;

  AssignNode(std::shared_ptr<Node> left,
             Token token,
             std::shared_ptr<Node> right);
  void accept(NodeVisitor& v) override;
  void accept(SymbolTableVisitor& v) override;
};

class VarNode : public Node {
 public:
  Token token;

  VarNode(Token token);
  void accept(NodeVisitor& v) override;
  void accept(SymbolTableVisitor& v) override;
};

class LiteralNode : public Node {
 public:
  Token token;

  LiteralNode(Token token);
  void accept(NodeVisitor& v) override;
  void accept(SymbolTableVisitor& v) override;
};

class CompoundNode : public Node {
 public:
  /* Token token; */
  std::vector<std::shared_ptr<Node>> children;

  CompoundNode(std::vector<std::shared_ptr<Node>> children);
  void accept(NodeVisitor& v) override;
  void accept(SymbolTableVisitor& v) override;
};

class ProgramNode : public Node {
 public:
  std::shared_ptr<Node> child;

  ProgramNode(std::shared_ptr<Node> child);
  void accept(NodeVisitor& v) override;
  void accept(SymbolTableVisitor& v) override;
};

class BlockNode : public Node {
 public:
  std::vector<std::shared_ptr<Node>> declarations;
  std::shared_ptr<Node> compound_statement;

  BlockNode(std::vector<std::shared_ptr<Node>> declarations,
            std::shared_ptr<Node> compound_statement);
  void accept(NodeVisitor& v) override;
  void accept(SymbolTableVisitor& v) override;
};

class VarDeclNode : public Node {
 public:
  std::shared_ptr<Node> var;
  std::shared_ptr<Node> type;

  VarDeclNode(std::shared_ptr<Node> var, std::shared_ptr<Node> type);
  void accept(NodeVisitor& v) override;
  void accept(SymbolTableVisitor& v) override;
};

class ProcedureDeclNode : public Node {
public:
  std::string name;
  std::shared_ptr<Node> block;

  ProcedureDeclNode(std::string name, std::shared_ptr<Node> block);
  void accept(NodeVisitor& v) override;
  void accept(SymbolTableVisitor& v) override;

};

class TypeNode : public Node {
 public:
  Token token;

  TypeNode(Token token);
  void accept(NodeVisitor& v) override;
  void accept(SymbolTableVisitor& v) override;
};

class NoOpNode : public Node {
 public:
  void accept(NodeVisitor& v) override;
  void accept(SymbolTableVisitor& v) override;
};

class Visitor {
 public:
  virtual void visit(BinaryNode& node) = 0;
  virtual void visit(UnaryNode& node) = 0;
  virtual void visit(AssignNode& node) = 0;
  virtual void visit(LiteralNode& node) = 0;
  virtual void visit(VarNode& node) = 0;
  virtual void visit(CompoundNode& node) = 0;
  virtual void visit(ProgramNode& node) = 0;
  virtual void visit(BlockNode& node) = 0;
  virtual void visit(VarDeclNode& node) = 0;
  virtual void visit(ProcedureDeclNode& node) = 0;
  virtual void visit(TypeNode& node) = 0;
  virtual void visit(NoOpNode& node) = 0;
};

class NodeVisitor : public Visitor {
 public:
  int value;
  std::string name;

  CallStack callstack;
  void visit(BinaryNode& node) override;
  void visit(UnaryNode& node) override;
  void visit(AssignNode& node) override;
  void visit(LiteralNode& node) override;
  void visit(VarNode& node) override;
  void visit(CompoundNode& node) override;
  void visit(ProgramNode& node) override;
  void visit(BlockNode& node) override;
  void visit(VarDeclNode& node) override;
  void visit(ProcedureDeclNode& node) override;
  void visit(TypeNode& node) override;
  void visit(NoOpNode& node) override;
};

class SymbolTableVisitor : public Visitor {
 public:
  int value;
  std::string name;
  SymbolTable symbols;

  void visit(BinaryNode& node) override;
  void visit(UnaryNode& node) override;
  void visit(AssignNode& node) override;
  void visit(LiteralNode& node) override;
  void visit(VarNode& node) override;
  void visit(CompoundNode& node) override;
  void visit(ProgramNode& node) override;
  void visit(BlockNode& node) override;
  void visit(VarDeclNode& node) override;
  void visit(ProcedureDeclNode& node) override;
  void visit(TypeNode& node) override;
  void visit(NoOpNode& node) override;
};
