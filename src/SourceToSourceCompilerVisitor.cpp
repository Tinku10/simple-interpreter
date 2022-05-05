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
    : level(0) {
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
  value = name;
}

void SourceToSourceCompilerVisitor::visit(AssignNode& node) {
  node.left->accept(*this);

  std::string var = name;

  node.right->accept(*this);

  std::cout << std::string(" ", level);
  std::cout << var << " := " << value << ";\n";
}

void SourceToSourceCompilerVisitor::visit(LiteralNode& node) {
  return;
}

void SourceToSourceCompilerVisitor::visit(CompoundNode& node) {
  for (auto& child : node.children) {
    child->accept(*this);
  }
}

void SourceToSourceCompilerVisitor::visit(ProgramNode& node) {
  std::cout << "PROGRAM "
            << std::dynamic_pointer_cast<VarNode>(node.var)->token.value << 0
            << ";\n";

  node.child->accept(*this);
}

void SourceToSourceCompilerVisitor::visit(BlockNode& node) {
  level += 1;

  for (auto& child : node.declarations) {
    child->accept(*this);
  }

  std::cout << std::string(" ", level);
  std::cout << "BEGIN; {Begin " << level << "}\n";
  node.compound_statement->accept(*this);
  std::cout << std::string(" ", level);
  std::cout << "END; {End of " << level << "}\n";

  level -= 1;
}

void SourceToSourceCompilerVisitor::visit(VarDeclNode& node) {
  std::string name = std::dynamic_pointer_cast<VarNode>(node.var)->token.value;
  std::string type
      = std::dynamic_pointer_cast<TypeNode>(node.type)->token.value;

  std::cout << std::string(" ", level);
  std::cout << "var " << name << level << ": " << type
            << ";\n";
}

void SourceToSourceCompilerVisitor::visit(ProcedureDeclNode& node) {
  std::vector<std::pair<std::string, std::string>> list;

  for (auto& child : node.params) {
    std::shared_ptr<ParamsNode> param
        = std::static_pointer_cast<ParamsNode>(child);
    std::string& name
        = std::static_pointer_cast<VarNode>(param->var)->token.value;
    std::string& type
        = std::static_pointer_cast<VarNode>(param->type)->token.value;

    list.push_back({name, type});
    child->accept(*this);
  }

  std::cout << std::string(" ", level);
  std::cout << "PROCEDURE " << node.name
            << level << "("
            << "";
  for (auto& child : list) {
    std::cout << child.first << ": " << child.second;
  }
  std::cout << ");\n";

  node.block->accept(*this);
}

void SourceToSourceCompilerVisitor::visit(ParamsNode& node) {
}

void SourceToSourceCompilerVisitor::visit(TypeNode& node) {
  return;
}

void SourceToSourceCompilerVisitor::visit(NoOpNode& node) {
  return;
}
