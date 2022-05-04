#include "Token.h"

Token::Token() {}

Token::~Token() {}

Token::Token(TokenType type,
             std::string&& value,
             uint start,
             uint end,
             uint line)
    : type(type), value(value), start(start), end(end), line(line) {
}

std::ostream& operator<<(std::ostream& cout, Token& token) {
  std::cout << "Token(" << (int)token.type << ", " << token.value << ")";
  return cout;
}
