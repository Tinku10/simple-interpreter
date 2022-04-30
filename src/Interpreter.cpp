#include "Interpreter.h"

Interpreter::Interpreter(Parser& parser, Visitor& visitor) : parser(parser), visitor(visitor) {}

void Interpreter::interpret() {
  std::shared_ptr<Node> root = parser.parse();
  root->accept(visitor);
}
