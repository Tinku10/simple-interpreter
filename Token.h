#include <iostream>
#include "TokenType.h"

class Token {
public:
  TokenType type;
  std::string value;
  uint start, end;
  uint line;

  Token(TokenType type, std::string&& value, uint start, uint end, uint line);
  ~Token();

  friend std::ostream& operator<<(std::ostream& cout, Token& token);
};
