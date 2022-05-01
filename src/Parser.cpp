#include "Parser.h"

Parser::Parser(Lexer& lexer) : lexer(lexer) {
}

void Parser::eat(TokenType type) {
  if (current_token.type != type) {
    throw std::invalid_argument("Unexpected token");
  }
  current_token = lexer.get_next_token();
}

std::shared_ptr<Node> Parser::factor() {
  if (current_token.type == TokenType::INTEGER_CONST
      || current_token.type == TokenType::REAL_CONST) {
    Token token = current_token;
    eat(current_token.type);
    return std::make_shared<LiteralNode>(LiteralNode(token));
  }

  if (current_token.type == TokenType::ID) {
    Token token = current_token;
    eat(TokenType::ID);
    return std::make_shared<VarNode>(VarNode(token));
  }

  if (current_token.type == TokenType::PLUS
      || current_token.type == TokenType::MINUS) {
    Token token = current_token;
    eat(current_token.type);
    return std::make_shared<UnaryNode>(UnaryNode(factor(), token));
  }

  if (current_token.type == TokenType::L_PAREN) {
    eat(TokenType::L_PAREN);
    std::shared_ptr<Node> node = expr();
    eat(TokenType::R_PAREN);

    return node;
  }

  return std::make_shared<NoOpNode>(NoOpNode());
}

std::shared_ptr<Node> Parser::term() {
  std::shared_ptr<Node> left = factor();

  while (current_token.type == TokenType::MULTIPLY
         || current_token.type == TokenType::FLOAT_DIV
         || current_token.type == TokenType::INT_DIV) {
    Token token = current_token;
    eat(current_token.type);
    left = std::make_shared<BinaryNode>(BinaryNode(left, token, factor()));
  }

  return left;
}

std::shared_ptr<Node> Parser::expr() {
  std::shared_ptr<Node> left = term();

  while (current_token.type == TokenType::PLUS
         || current_token.type == TokenType::MINUS) {
    Token token = current_token;
    eat(current_token.type);
    left = std::make_shared<BinaryNode>(BinaryNode(left, token, term()));
  }

  return left;
}

std::shared_ptr<Node> Parser::statement() {
  if (current_token.type == TokenType::BEGIN) {
    return compound();
  }

  if (current_token.type == TokenType::ID) {
    std::shared_ptr<Node> left = factor();

    Token token = current_token;
    eat(TokenType::EQUAL);

    return std::make_shared<AssignNode>(AssignNode(left, token, expr()));
  }

  return std::make_shared<NoOpNode>(NoOpNode());
}

std::shared_ptr<Node> Parser::statement_list() {
  std::vector<std::shared_ptr<Node>> v;
  v.push_back(statement());

  while (current_token.type == TokenType::SEMI) {
    eat(current_token.type);
    v.push_back(statement());
  }

  return std::make_shared<CompoundNode>(CompoundNode(v));
}

std::shared_ptr<Node> Parser::compound() {
  eat(TokenType::BEGIN);
  std::shared_ptr<Node> node = statement_list();
  eat(TokenType::END);

  return node;
}

std::shared_ptr<Node> Parser::type() {
  Token token = current_token;
  if (current_token.type == TokenType::INTEGER) {
    eat(TokenType::INTEGER);
  } else if (current_token.type == TokenType::REAL) {
    eat(TokenType::REAL);
  } else {
    std::invalid_argument("Invalid type");
  }
  return std::make_shared<TypeNode>(TypeNode(token));
}

std::vector<std::shared_ptr<Node>> Parser::var_declaration() {
  std::vector<std::shared_ptr<Node>> v;

  v.push_back(factor());

  while (current_token.type == TokenType::COMMA) {
    eat(current_token.type);
    v.push_back(factor());
  }

  eat(TokenType::COLON);

  std::shared_ptr<Node> node = type();

  std::vector<std::shared_ptr<Node>> declarations;

  for (auto& child : v) {
    declarations.emplace_back(
        std::make_shared<VarDeclNode>(VarDeclNode(child, node)));
  }

  return declarations;
}

std::vector<std::shared_ptr<Node>> Parser::declarations() {
  eat(TokenType::VAR);
  std::vector<std::shared_ptr<Node>> v;

  while (current_token.type == TokenType::ID) {
    std::vector<std::shared_ptr<Node>> declarations = var_declaration();
    eat(TokenType::SEMI);

    for (auto& child : declarations) v.push_back(child);
  }

  return v;
}

std::shared_ptr<Node> Parser::block() {
  return std::make_shared<BlockNode>(BlockNode(declarations(), compound()));
}

std::shared_ptr<Node> Parser::program() {
  eat(TokenType::PROGRAM);
  std::shared_ptr<Node> var = factor();
  eat(TokenType::SEMI);
  std::shared_ptr<Node> node = block();
  eat(TokenType::DOT);
  eat(TokenType::ENDOFFILE);

  return node;
}

std::shared_ptr<Node> Parser::parse() {
  current_token = lexer.get_next_token();
  return program();
}
