#include "Interpreter.h"

Interpreter::Interpreter(Parser& parser)
    : parser(parser) {
}

void Interpreter::interpret() {
  std::shared_ptr<Node> root = parser.parse();

  NodeVisitor visitor;
  SymbolTableVisitor symbol_visitor;
  TypeCheckerVisitor type_checker;
  SourceToSourceCompilerVisitor source_to_source_compiler_visitor;

  root->accept(symbol_visitor);
  root->accept(type_checker);
  root->accept(source_to_source_compiler_visitor);
  root->accept(visitor);
}
