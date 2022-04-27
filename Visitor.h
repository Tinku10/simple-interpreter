#include "Token.h"
#include <iostream>

class Visitor;

template <class T> class Node {
public:
  std::shared_ptr<Token> token;
  virtual T accept(Visitor &v) = 0;
};

template <class T> class BinaryNode : public Node<T> {
public:
  std::shared_ptr<Node<T>> left;
  std::shared_ptr<Token> token;
  std::shared_ptr<Node<T>> right;

  BinaryNode(std::shared_ptr<Node<T>> left, std::shared_ptr<Token> token,
             std::shared_ptr<Node<T>> right);

  T accept(Visitor &v) override;
};

template <class T> class UnaryNode : public Node<T> {
public:
  std::shared_ptr<Node<T>> node;
  std::shared_ptr<Token> token;

  UnaryNode(std::shared_ptr<Node<T>> node, std::shared_ptr<Token> token);
  T accept(Visitor &v) override;
};

template <class T> class AssignNode : public Node<T> {
public:
  std::shared_ptr<Node<T>> left;
  std::shared_ptr<Token> token;
  std::shared_ptr<Node<T>> right;

  AssignNode(std::shared_ptr<Node<T>> left, std::shared_ptr<Token> token, std::shared_ptr<Node<T>> right);
  T accept(Visitor &v) override;
};

template <class T> class VarNode : public Node<T> {
public:
  std::shared_ptr<Token> token;

  VarNode(std::shared_ptr<Token> token);
  T accept(Visitor &v) override;
};

template <class T> class LiteralNode : public Node<T> {
public:
  std::shared_ptr<Token> token;

  LiteralNode(std::shared_ptr<Token> token);
  T accept(Visitor &v) override;
};

class Visitor {
public:
  int visit(BinaryNode<int> &node);
  int visit(UnaryNode<int> &node);
  void visit(AssignNode<void> &node);
  int visit(LiteralNode<int> &node);
  std::string visit(LiteralNode<std::string> &node);
  int visit(VarNode<int>& node);
};
