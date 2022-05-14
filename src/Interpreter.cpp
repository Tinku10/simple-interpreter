#include "Interpreter.h"

Interpreter::Interpreter(Parser& parser)
    : parser(parser) {
}

void Interpreter::interpret() {
  std::shared_ptr<Node> root = parser.parse();

  NodeVisitor visitor;
  SymbolTableVisitor symbol_visitor;
  SourceToSourceCompilerVisitor source_to_source_compiler_visitor;

  root->accept(symbol_visitor);
  root->accept(source_to_source_compiler_visitor);
  root->accept(visitor);
}
