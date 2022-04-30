#include "Parser.h"

Parser::Parser(Lexer& lexer) : lexer(lexer) {}

void Parser::eat(TokenType type) {
  if (current_token.type != type) {
    throw std::invalid_argument("Unexpected token");
  }
  current_token = lexer.get_next_token();
}

std::shared_ptr<Node> Parser::factor() {
  if (current_token.type == TokenType::NUMBER) {
    Token token = current_token;
    eat(TokenType::NUMBER);
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
         || current_token.type == TokenType::DIVIDE) {
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

std::shared_ptr<Node> Parser::program() {
  std::shared_ptr<Node> node = compound();
  eat(TokenType::DOT);
  eat(TokenType::ENDOFFILE);
  
  return node;
}

std::shared_ptr<Node> Parser::parse() {
  current_token = lexer.get_next_token();
  return program();
}
