#include "Lexer.h"

std::unordered_map<std::string, TokenType> reserved_keywords
    = {{"BEGIN", TokenType::BEGIN}, {"END", TokenType::END},
       {"VAR", TokenType::VAR},     {"PROGRAM", TokenType::PROGRAM},
       {"DIV", TokenType::INT_DIV}, {"INTEGER", TokenType::INTEGER},
       {"REAL", TokenType::REAL},   {"PROCEDURE", TokenType::PROCEDURE},
       {"STRING", TokenType::STRING}};

Lexer::Lexer(std::string& source) : source(source), line(0), col(0) {
}

void Lexer::advance() {
  if (index < source.length() && source[index] == '\n') {
    line++;
    col = 0;
  }

  index++;
  col++;
}

char Lexer::peek() {
  if (index < source.length()) return source[index + 1];

  throw error(ErrorCode::PEEK_PAST_LENGTH);
  /* throw std::invalid_argument("Cannot peek past length of source"); */
}

void Lexer::trim_white_space() {
  while (index < source.length() && std::isspace(source[index])) advance();
}

void Lexer::skip_comments() {
  while (index < source.length() && source[index] != '}') advance();
  advance();

  if (isspace(source[index])) trim_white_space();
}

Token Lexer::number() {
  uint start = col;

  std::string result = "";
  while (index < source.length() && isdigit(source[index])) {
    result += source[index];
    advance();
  }

  if (index < source.length() && source[index] == '.') {
    result += source[index];
    advance();
    while (index < source.length() && isdigit(source[index])) {
      result += source[index];
      advance();
    }

    return Token(TokenType::REAL_CONST, std::move(result), start, col + 1,
                 line);
  }

  return Token(TokenType::INTEGER_CONST, std::move(result), start, col + 1,
               line);
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
  uint start = col;

  switch (type) {
    case TokenType::QUOTES:
      return Token(TokenType::STRING_CONST, string(), start + 1, index - 1, line);
    case TokenType::ID: {
      std::string identifier = id();
      if (reserved_keywords.count(identifier))
        return Token(reserved_keywords[identifier], std::move(identifier),
                     start, index, line);
      return Token(type, std::move(identifier), start, index, line);
    }
    case TokenType::PLUS:
      advance();
      return Token(type, "+", start, start + 1, line);
    case TokenType::MINUS:
      advance();
      return Token(type, "-", start, start + 1, line);
    case TokenType::MULTIPLY:
      advance();
      return Token(type, "*", start, start + 1, line);
    case TokenType::FLOAT_DIV:
      advance();
      return Token(type, "/", start, start + 1, line);
    case TokenType::EQUAL:
      advance();
      advance();
      return Token(type, ":=", start, start + 2, line);
    case TokenType::EQUAL_EQUAL:
      advance();
      advance();
      return Token(type, "==", start, start + 2, line);
    case TokenType::GREATER_THAN:
      advance();
      return Token(type, ">", start, start + 1, line);
    case TokenType::GT_EQUAL:
      advance();
      advance();
      return Token(type, ">=", start, start + 2, line);
    case TokenType::LESS_THAN:
      advance();
      return Token(type, "<", start, start + 1, line);
    case TokenType::LT_EQUAL:
      advance();
      advance();
      return Token(type, "<=", start, start + 2, line);
    case TokenType::NOT_EQUAL:
      advance();
      advance();
      return Token(type, "!=", start, start + 2, line);
    case TokenType::DOT:
      advance();
      return Token(type, ".", start, start + 1, line);
    case TokenType::SEMI:
      advance();
      return Token(type, ";", start, start + 1, line);
    case TokenType::COLON:
      advance();
      return Token(type, ":", start, start + 1, line);
    case TokenType::COMMA:
      advance();
      return Token(type, ",", start, start + 1, line);
    case TokenType::L_PAREN:
      advance();
      return Token(type, "(", start, start + 1, line);
    case TokenType::R_PAREN:
      advance();
      return Token(type, ")", start, start + 1, line);
    default:
      throw error(ErrorCode::UNKNOWN_CHAR);
      /* throw std::invalid_argument("Invalid token"); */
  }
}

Token Lexer::get_next_token() {
  if (index > source.length() - 1) {
    return Token(TokenType::ENDOFFILE, "EOF", index, index + 1, line);
  }

  char curr = source[index];

  if (isspace(curr)) trim_white_space();

  while (source[index] == '{') {
    skip_comments();
  }

  if (index > source.length() - 1) {
    return Token(TokenType::ENDOFFILE, "EOF", index, index + 1, line);
  }

  curr = source[index];

  if (isdigit(curr)) return number();

  curr = source[index];

  if (isalnum(curr)) return add_token(TokenType::ID);

  curr = source[index];

  switch (curr) {
    case '+': return add_token(TokenType::PLUS);
    case '-': return add_token(TokenType::MINUS);
    case '*': return add_token(TokenType::MULTIPLY);
    case '/': return add_token(TokenType::FLOAT_DIV);
    case ':':
      if (peek() == '=')
        return add_token(TokenType::EQUAL);
      else
        return add_token(TokenType::COLON);
    case '=':
      if (peek() == '=') return add_token(TokenType::EQUAL_EQUAL);
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
    case '"': return add_token(TokenType::QUOTES);
    case '.': return add_token(TokenType::DOT);
    case ';': return add_token(TokenType::SEMI);
    case ',': return add_token(TokenType::COMMA);
    case '(': return add_token(TokenType::L_PAREN);
    case ')': return add_token(TokenType::R_PAREN);
    default:
      throw error(ErrorCode::UNKNOWN_CHAR);
      /* throw std::invalid_argument("Invalid token"); */
  }
}

Exception Lexer::error(ErrorCode code) {
  std::string msg = "";

  switch (code) {
    case ErrorCode::PEEK_PAST_LENGTH:
      msg = "peek past the source code length";
      break;
    case ErrorCode::UNKNOWN_CHAR: msg = "unknown character"; break;
    default: msg = "unknown error while lexing";
  }
  msg = msg + "\nerror at line " + std::to_string(this->line) + " column "
        + std::to_string(this->col);
  throw Exception(code, msg);
}
