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

  Token add_token(TokenType type);

  char peek();

  std::string number();

  std::string string();

  std::string id();

  Token get_next_token();
};

std::unordered_map<std::string, TokenType> reserved_keywords;

reserved_keywords["and"] = TokenType::AND;
reserved_keywords["or"] = TokenType::OR;
reserved_keywords["for"] = TokenType::FOR;
reserved_keywords["while"] = TokenType::WHILE;
reserved_keywords["if"] = TokenType::IF;
