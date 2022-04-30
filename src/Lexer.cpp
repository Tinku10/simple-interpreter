#include "Lexer.h"

std::unordered_map<std::string, TokenType> reserved_keywords = {
  {"BEGIN", TokenType::BEGIN},
  {"END", TokenType::END}
};

Lexer::Lexer(std::string& source) : source(source) {
}

void Lexer::advance() {
  index++;
}

char Lexer::peek() {
  if (index < source.length()) return source[index + 1];

  throw std::invalid_argument("Cannot peek past length of source");
}

void Lexer::trim_white_space() {
  while (index < source.length() && std::isspace(source[index])) index++;
}

std::string Lexer::number() {
  std::string result = "";
  while (index < source.length() && isdigit(source[index])) {
    result += source[index];
    advance();
  }

  return result;
}

std::string Lexer::id() {
  std::string result = "";
  while (index < source.length() && isalnum(source[index])) {
    result += source[index];
    advance();
  }

  return result;
}

std::string Lexer::string() {
  advance();

  std::string result = "";
  while (index < source.length() && source[index] != '"') {
    if (source[index] == '\n') {
      line++;
      advance();
    }
    result += source[index];
    advance();
  }

  if (source[index] != '"') {
    throw std::invalid_argument("Unternimated string");
  }

  advance();

  return result;
}

Token Lexer::add_token(TokenType type) {
  uint start = index;

  switch (type) {
    case TokenType::STRING:
      return Token(type, string(), start + 1, index - 1, line);
    case TokenType::NUMBER: return Token(type, number(), start, index, line);
    case TokenType::ID: {
      std::string identifier = id();
      if (reserved_keywords.count(identifier))
        return Token(reserved_keywords[identifier], std::move(identifier),
                     start, index, line);
      return Token(type, std::move(identifier), start, index, line);
    }
    case TokenType::PLUS:
      advance();
      return Token(type, "+", start, index, line);
    case TokenType::MINUS:
      advance();
      return Token(type, "-", start, index, line);
    case TokenType::MULTIPLY:
      advance();
      return Token(type, "*", start, index, line);
    case TokenType::DIVIDE:
      advance();
      return Token(type, "/", start, index, line);
    case TokenType::EQUAL:
      advance();
      advance();
      return Token(type, ":=", start, index, line);
    case TokenType::EQUAL_EQUAL:
      advance();
      advance();
      return Token(type, "==", start, index, line);
    case TokenType::GREATER_THAN:
      advance();
      return Token(type, ">", start, index, line);
    case TokenType::GT_EQUAL:
      advance();
      advance();
      return Token(type, ">=", start, index, line);
    case TokenType::LESS_THAN:
      advance();
      return Token(type, "<", start, index, line);
    case TokenType::LT_EQUAL:
      advance();
      advance();
      return Token(type, "<=", start, index, line);
    case TokenType::NOT_EQUAL:
      advance();
      advance();
      return Token(type, "!=", start, index, line);
    case TokenType::DOT:
      advance();
      return Token(type, ".", start, index, line);
    case TokenType::SEMI:
      advance();
      return Token(type, ";", start, index, line);
    default: throw std::invalid_argument("Invalid token");
  }
}

Token Lexer::get_next_token() {
  if (index > source.length() - 1) {
    return Token(TokenType::ENDOFFILE, "EOF", index, index + 1, line);
  }

  char curr = source[index];

  if (isspace(curr)) trim_white_space();

  curr = source[index];

  if (isdigit(curr)) return add_token(TokenType::NUMBER);

  curr = source[index];

  if (isalnum(curr)) return add_token(TokenType::ID);

  curr = source[index];

  switch (curr) {
    case '+': return add_token(TokenType::PLUS);
    case '-': return add_token(TokenType::MINUS);
    case '*': return add_token(TokenType::MULTIPLY);
    case '/': return add_token(TokenType::DIVIDE);
    case ':':
      if (peek() == '=')
        return add_token(TokenType::EQUAL);
    case '=':
      if (peek() == '=')
        return add_token(TokenType::EQUAL_EQUAL);
      /* else */
      /*   return add_token(TokenType::EQUAL); */
    case '>':
      if (peek() == '=')
        return add_token(TokenType::GT_EQUAL);
      else
        return add_token(TokenType::GREATER_THAN);
    case '<':
      if (peek() == '=')
        return add_token(TokenType::LT_EQUAL);
      else
        return add_token(TokenType::LESS_THAN);
    case '!':
      if (peek() == '=') return add_token(TokenType::NOT_EQUAL);
    case '"': return add_token(TokenType::STRING);
    case '.': return add_token(TokenType::DOT);
    case ';': return add_token(TokenType::SEMI);
    default: throw std::invalid_argument("Invalid token");
  }
}
