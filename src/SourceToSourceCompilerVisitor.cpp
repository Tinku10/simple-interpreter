#include <unordered_map>

#include "Visitor.h"

void BinaryNode::accept(SourceToSourceCompilerVisitor& v) {
  v.visit(*this);
}

void UnaryNode::accept(SourceToSourceCompilerVisitor& v) {
  v.visit(*this);
}

void AssignNode::accept(SourceToSourceCompilerVisitor& v) {
  v.visit(*this);
}

void VarNode::accept(SourceToSourceCompilerVisitor& v) {
  v.visit(*this);
}

void LiteralNode::accept(SourceToSourceCompilerVisitor& v) {
  v.visit(*this);
}

void CompoundNode::accept(SourceToSourceCompilerVisitor& v) {
  v.visit(*this);
}

void ProgramNode::accept(SourceToSourceCompilerVisitor& v) {
  v.visit(*this);
}

void BlockNode::accept(SourceToSourceCompilerVisitor& v) {
  v.visit(*this);
}

void VarDeclNode::accept(SourceToSourceCompilerVisitor& v) {
  v.visit(*this);
}

void ProcedureDeclNode::accept(SourceToSourceCompilerVisitor& v) {
  v.visit(*this);
}

void ProcedureCallNode::accept(SourceToSourceCompilerVisitor& v) {
  v.visit(*this);
}

void ParamsNode::accept(SourceToSourceCompilerVisitor& v) {
  v.visit(*this);
}

void TypeNode::accept(SourceToSourceCompilerVisitor& v) {
  v.visit(*this);
}

void NoOpNode::accept(SourceToSourceCompilerVisitor& v) {
  v.visit(*this);
}

SourceToSourceCompilerVisitor::SourceToSourceCompilerVisitor()
    : level(0),
      current_scope(std::make_shared<ScopedSymbolTable>(
          ScopedSymbolTable("BUILTIN", 0))) {
}

void SourceToSourceCompilerVisitor::visit(BinaryNode& node) {
  node.left->accept(*this);
  std::string left = value;
  node.right->accept(*this);
  std::string right = value;

  value = left + " " + node.token.value + " " + right;
}

void SourceToSourceCompilerVisitor::visit(UnaryNode& node) {
  node.node->accept(*this);
}

void SourceToSourceCompilerVisitor::visit(VarNode& node) {
  name = node.token.value;
  std::shared_ptr<SymbolWithScope> symbol = current_scope->at(name);
  std::shared_ptr<SymbolWithScope> type
      = current_scope->at(symbol->symbol->type->name);
  value = "<" + name + std::to_string(symbol->scope_level) + ":"
          + symbol->symbol->type->name + std::to_string(type->scope_level)
          + ">";
}

void SourceToSourceCompilerVisitor::visit(AssignNode& node) {
  node.left->accept(*this);

  std::string var = name;

  node.right->accept(*this);

  std::cout << std::string(level * 2, ' ');
  std::cout << var + std::to_string(current_scope->scope_level)
            << " := " << value << ";\n";
}

void SourceToSourceCompilerVisitor::visit(LiteralNode& node) {
  if (node.token.type == TokenType::STRING_CONST)
    value = "\"" + node.token.value + "\"";
  else
    value = node.token.value;
}

void SourceToSourceCompilerVisitor::visit(CompoundNode& node) {
  level += 1;

  for (auto& child : node.children) {
    child->accept(*this);
  }

  level -= 1;
}

void SourceToSourceCompilerVisitor::visit(ProgramNode& node) {
  std::cout << "PROGRAM "
            << std::dynamic_pointer_cast<VarNode>(node.var)->token.value
            << current_scope->scope_level << ";\n";

  current_scope = std::make_shared<ScopedSymbolTable>(
      ScopedSymbolTable("GLOBAL", 1, current_scope));

  node.child->accept(*this);

  current_scope = current_scope->parent_scope;
}

void SourceToSourceCompilerVisitor::visit(BlockNode& node) {
  level += 1;

  for (auto& child : node.declarations) {
    child->accept(*this);
  }

  std::cout << std::string(level * 2, ' ');
  std::cout << "BEGIN; {Begin " << current_scope->scope_name << "}\n";
  node.compound_statement->accept(*this);
  std::cout << std::string(level * 2, ' ');
  std::cout << "END; {End of " << current_scope->scope_name << "}\n";

  level -= 1;
}

void SourceToSourceCompilerVisitor::visit(VarDeclNode& node) {
  std::string name = std::dynamic_pointer_cast<VarNode>(node.var)->token.value;
  std::string type
      = std::dynamic_pointer_cast<TypeNode>(node.type)->token.value;

  std::cout << std::string(level * 2, ' ');
  std::cout << "var " << name << level << ": ";

  node.type->accept(*this);

  std::cout << "\n";

  std::shared_ptr<SymbolWithScope> symbol = current_scope->at(type);

  current_scope->add(
      std::make_shared<VarTypeSymbol>(VarTypeSymbol(name, symbol->symbol)));
}

void SourceToSourceCompilerVisitor::visit(ProcedureDeclNode& node) {
  current_scope = std::make_shared<ScopedSymbolTable>(ScopedSymbolTable(
      std::move(node.name), current_scope->scope_level + 1, current_scope));

  std::vector<std::shared_ptr<Symbol>> list;

  std::cout << std::string(level * 2, ' ');
  std::cout << "PROCEDURE " << node.name << level << "("
            << "";

  for (auto& child : node.params) {
    std::shared_ptr<ParamsNode> param
        = std::static_pointer_cast<ParamsNode>(child);
    std::string& name
        = std::static_pointer_cast<VarNode>(param->var)->token.value;
    std::string& type
        = std::static_pointer_cast<TypeNode>(param->type)->token.value;

    std::shared_ptr<VarTypeSymbol> var = std::make_shared<VarTypeSymbol>(
        VarTypeSymbol(name, current_scope->at(type)->symbol));

    current_scope->add(var);
    list.push_back(var);
    child->accept(*this);

    uint is_last = node.params.size() - (&child - &node.params[0]) - 1;

    if (is_last) std::cout << "; ";
  }

  std::cout << ");\n";

  std::shared_ptr<ProcedureSymbol> symbol
      = std::make_shared<ProcedureSymbol>(ProcedureSymbol(node.name, list));

  current_scope->parent_scope->add(symbol);

  node.block->accept(*this);

  current_scope = current_scope->parent_scope;
}

void SourceToSourceCompilerVisitor::visit(ProcedureCallNode& node) {
  std::cout << std::string(level * 2, ' ');
  std::cout << node.name << "(";

  for (auto& child : node.params) {
    child->accept(*this);

    std::cout << value;

    bool not_last = node.params.size() - (&child - &node.params[0]) - 1;
    if (not_last) std::cout << ", ";
  }

  std::cout << ")";
}

void SourceToSourceCompilerVisitor::visit(ParamsNode& node) {
  /* node.var->accept(*this); */
  std::string& name = std::static_pointer_cast<VarNode>(node.var)->token.value;
  std::cout << name << ": ";
  node.type->accept(*this);
}

void SourceToSourceCompilerVisitor::visit(TypeNode& node) {
  std::shared_ptr<SymbolWithScope> symbol = current_scope->at(node.token.value);
  std::cout << node.token.value << symbol->scope_level;
}

void SourceToSourceCompilerVisitor::visit(NoOpNode& node) {
  return;
}
