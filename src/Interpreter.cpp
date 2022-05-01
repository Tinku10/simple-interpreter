#include "Interpreter.h"

Interpreter::Interpreter(Parser& parser,
                         NodeVisitor& visitor,
                         SymbolTableVisitor& symbol_visitor)
    : parser(parser), visitor(visitor), symbol_visitor(symbol_visitor) {
}

void Interpreter::interpret() {
  std::shared_ptr<Node> root = parser.parse();
  root->accept(symbol_visitor);
  /* root->accept(visitor); */
}
