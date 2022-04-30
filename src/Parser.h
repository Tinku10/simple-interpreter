#pragma once
#include "Lexer.h"
#include "Visitor.h"

/**
 * Grammar
 * program: compound . 
 * compound: begin statement_list end
 * statement_list: statement SEMI statement_list | compound
 * statement: var ASSIGN expr
 * var: id
 * expr: term ((PLUS | MINUS) term)*
 * term: id ((MUL | DIV) id)*
 * id: number | (PLUS | MINUS | BANG)* id | LPAREN expr RPAREN
 */
class Parser {
public:
  Token current_token;
  Lexer lexer;
  Parser(Lexer& lexer); 

  void eat(TokenType type);

  std::shared_ptr<Node> factor();
  std::shared_ptr<Node> term();
  std::shared_ptr<Node> expr();
  std::shared_ptr<Node> var();
  std::shared_ptr<Node> statement();
  std::shared_ptr<Node> statement_list();
  std::shared_ptr<Node> compound();
  std::shared_ptr<Node> program();

  std::shared_ptr<Node> parse();
};
