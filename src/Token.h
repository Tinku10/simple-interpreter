#pragma once

#include <iostream>
#include "TokenType.h"
#include "Exceptions.h"

class Token {
public:
  TokenType type;
  std::string value;
  uint start, end;
  uint line;

  Token();
  Token(TokenType type, std::string&& value, uint start, uint end, uint line);
  ~Token();

  friend std::ostream& operator<<(std::ostream& cout, Token& token);

  void error();
};
