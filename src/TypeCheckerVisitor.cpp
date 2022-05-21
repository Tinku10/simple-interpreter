#include <unordered_map>

#include "Visitor.h"

void BinaryNode::accept(TypeCheckerVisitor& v) {
  v.visit(*this);
}

void UnaryNode::accept(TypeCheckerVisitor& v) {
  v.visit(*this);
}

void AssignNode::accept(TypeCheckerVisitor& v) {
  v.visit(*this);
}

void VarNode::accept(TypeCheckerVisitor& v) {
  v.visit(*this);
}

void LiteralNode::accept(TypeCheckerVisitor& v) {
  v.visit(*this);
}

void CompoundNode::accept(TypeCheckerVisitor& v) {
  v.visit(*this);
}

void ProgramNode::accept(TypeCheckerVisitor& v) {
  v.visit(*this);
}

void BlockNode::accept(TypeCheckerVisitor& v) {
  v.visit(*this);
}

void VarDeclNode::accept(TypeCheckerVisitor& v) {
  v.visit(*this);
}

void ProcedureDeclNode::accept(TypeCheckerVisitor& v) {
  v.visit(*this);
}

void ProcedureCallNode::accept(TypeCheckerVisitor& v) {
  v.visit(*this);
}

void ParamsNode::accept(TypeCheckerVisitor& v) {
  v.visit(*this);
}

void TypeNode::accept(TypeCheckerVisitor& v) {
  v.visit(*this);
}

void NoOpNode::accept(TypeCheckerVisitor& v) {
  v.visit(*this);
}

TypeCheckerVisitor::TypeCheckerVisitor()
    : current_scope(
        std::make_shared<ScopedSymbolTable>(ScopedSymbolTable("BUILTIN", 0))) {
}

TokenType TypeCheckerVisitor::typecheck(TokenType& left,
                                        Token& op,
                                        TokenType& right) {
  TokenType op_type = op.type;

  if (left == TokenType::INTEGER_CONST || left == TokenType::INTEGER) {
    if (right == TokenType::INTEGER_CONST || right == TokenType::INTEGER) {
      if (op_type == TokenType::PLUS || op_type == TokenType::MINUS
          || op_type == TokenType::MULTIPLY || op_type == TokenType::DIVIDE)
        return TokenType::INTEGER;
      else if (op_type == TokenType::EQUAL)
        return left;
      throw error(ErrorCode::UNEXPECTED_TOKEN, op);
    } else if (right == TokenType::REAL_CONST || right == TokenType::REAL) {
      if (op_type == TokenType::PLUS || op_type == TokenType::MINUS
          || op_type == TokenType::MULTIPLY || op_type == TokenType::DIVIDE)
        return TokenType::REAL;
      else if (op_type == TokenType::EQUAL)
        return left;
      throw error(ErrorCode::UNEXPECTED_TOKEN, op);
    } else
      throw error(ErrorCode::UNSUPPORTED_OP_ON_TYPES, op);
  } else if (left == TokenType::REAL_CONST || left == TokenType::REAL) {
    if (right == TokenType::INTEGER_CONST || right == TokenType::INTEGER) {
      if (op_type == TokenType::PLUS || op_type == TokenType::MINUS
          || op_type == TokenType::MULTIPLY || op_type == TokenType::DIVIDE)
        return TokenType::REAL;
      else if (op_type == TokenType::EQUAL)
        return left;
      throw error(ErrorCode::UNEXPECTED_TOKEN, op);
    } else if (right == TokenType::REAL_CONST || left == TokenType::REAL) {
      if (op_type == TokenType::PLUS || op_type == TokenType::MINUS
          || op_type == TokenType::MULTIPLY || op_type == TokenType::DIVIDE)
        return TokenType::REAL;
      else if (op_type == TokenType::EQUAL)
        return left;
      throw error(ErrorCode::UNEXPECTED_TOKEN, op);
    } else
      throw error(ErrorCode::UNSUPPORTED_OP_ON_TYPES, op);
  } else if (left == TokenType::STRING_CONST || left == TokenType::STRING) {
    if (right == TokenType::STRING_CONST || right == TokenType::STRING) {
      if (op_type == TokenType::PLUS)
        return TokenType::STRING;
      else if (op_type == TokenType::EQUAL)
        return left;
      else
        throw error(ErrorCode::UNSUPPORTED_OP_ON_TYPES, op);
    } else {
      throw error(ErrorCode::UNSUPPORTED_OP_ON_TYPES, op);
    }
  } else
    throw error(ErrorCode::UNEXPECTED_TOKEN, op);
}

void TypeCheckerVisitor::visit(BinaryNode& node) {
  node.left->accept(*this);
  TokenType left = literal_type;
  node.right->accept(*this);
  TokenType right = literal_type;

  typecheck(left, node.token, right);
}

void TypeCheckerVisitor::visit(UnaryNode& node) {
  node.node->accept(*this);
}

void TypeCheckerVisitor::visit(VarNode& node) {
  std::shared_ptr<Symbol> symbol = node.type_symbol;

  std::shared_ptr<BuiltInTypeSymbol> builtin_type
      = std::dynamic_pointer_cast<BuiltInTypeSymbol>(symbol->type);

  literal_type = builtin_type->datatype;
}

void TypeCheckerVisitor::visit(AssignNode& node) {
  node.left->accept(*this);
  TokenType left = literal_type;
  node.right->accept(*this);
  TokenType right = literal_type;

  typecheck(left, node.token, right);

  node.left_type = left;
}

void TypeCheckerVisitor::visit(LiteralNode& node) {
  literal_type = node.token.type;
}

void TypeCheckerVisitor::visit(CompoundNode& node) {
  for (auto& child : node.children) {
    child->accept(*this);
  }
}

void TypeCheckerVisitor::visit(ProgramNode& node) {
  current_scope = std::make_shared<ScopedSymbolTable>(
      ScopedSymbolTable("GLOBAL", 1, current_scope));

  node.child->accept(*this);

  current_scope = current_scope->parent_scope;
}

void TypeCheckerVisitor::visit(BlockNode& node) {
  for (auto& child : node.declarations) {
    child->accept(*this);
  }

  node.compound_statement->accept(*this);
}

void TypeCheckerVisitor::visit(VarDeclNode& node) {
  std::string name = std::dynamic_pointer_cast<VarNode>(node.var)->token.value;
  std::string type
      = std::dynamic_pointer_cast<TypeNode>(node.type)->token.value;

  std::shared_ptr<SymbolWithScope> symbol = current_scope->at(type);

  current_scope->add(
      std::make_shared<VarTypeSymbol>(VarTypeSymbol(name, symbol->symbol)));
}

void TypeCheckerVisitor::visit(ProcedureDeclNode& node) {
  current_scope = std::make_shared<ScopedSymbolTable>(ScopedSymbolTable(
      std::move(node.name), current_scope->scope_level + 1, current_scope));

  std::vector<std::shared_ptr<Symbol>> list;

  for (auto& child : node.params) {
    std::shared_ptr<ParamsNode> param
        = std::static_pointer_cast<ParamsNode>(child);
    std::string& name
        = std::dynamic_pointer_cast<VarNode>(param->var)->token.value;
    std::string& type
        = std::dynamic_pointer_cast<TypeNode>(param->type)->token.value;

    std::shared_ptr<VarTypeSymbol> var = std::make_shared<VarTypeSymbol>(
        VarTypeSymbol(name, current_scope->at(type)->symbol));

    current_scope->add(var);
    list.push_back(var);
    child->accept(*this);
  }

  std::shared_ptr<ProcedureSymbol> symbol
      = std::make_shared<ProcedureSymbol>(ProcedureSymbol(node.name, list));

  symbol->block = node.block;

  current_scope->parent_scope->add(symbol);

  node.block->accept(*this);

  current_scope = current_scope->parent_scope;
}

void TypeCheckerVisitor::visit(ProcedureCallNode& node) {
  std::shared_ptr<SymbolWithScope> symbol
      = current_scope->at(node.name, (uint)node.params.size());

  node.procedure_symbol
      = std::dynamic_pointer_cast<ProcedureSymbol>(symbol->symbol);

  for (auto& child : node.params) {
    child->accept(*this);
  }
}

void TypeCheckerVisitor::visit(ParamsNode& node) {
}

void TypeCheckerVisitor::visit(TypeNode& node) {
  return;
}

void TypeCheckerVisitor::visit(NoOpNode& node) {
  return;
}

Exception TypeCheckerVisitor::error(ErrorCode code, Token& token) {
  std::string msg = "";
  switch (code) {
    case ErrorCode::UNSUPPORTED_OP_ON_TYPES:
      msg = "unsupported operation between types";
      break;
    case ErrorCode::UNEXPECTED_TOKEN: msg = "unexpected token"; break;
    case ErrorCode::IDENTIFIER_NOT_FOUND: msg = "identifier not found"; break;
    default: msg = "unknown error\n";
  }

  msg = "type error at column" + std::to_string(token.start) + " line"
        + std::to_string(token.line) + msg;
  throw Exception(code, msg);
}
