#include "Token.h"
#include<unordered_map>

class Lexer {
public:
  std::string source;
  uint index = 0;
  uint line = 0;

  Lexer(std::string& source);

  void advance(); 

  void trim_white_space();

  void skip_comments();

  Token add_token(TokenType type);

  char peek();

  Token number();

  std::string string();

  std::string id();

  Token get_next_token();
};
