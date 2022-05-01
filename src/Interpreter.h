#include "Parser.h"

class Interpreter {
public:
  Parser parser;
  NodeVisitor visitor;
  SymbolTableVisitor symbol_visitor;

  Interpreter(Parser& parser, NodeVisitor& visitor, SymbolTableVisitor& symbol_visitor);

  void interpret();
};
