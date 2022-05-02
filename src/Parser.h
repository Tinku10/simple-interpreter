#pragma once
#include "Lexer.h"
#include "Visitor.h"

/**
 * Grammar
 * ============================================================================
 * program: PROGRAM variable SEMI block DOT
 * block: declarations compound
 * declarations: var (variable_declaration SEMI)+ | (PROCEDURE id SEMI block SEMI)* | EMPTY
 * variable_declaration: id (COMMA id)* COLON type
 * type: INTEGER | REAL
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
  std::shared_ptr<Node> block();
  std::vector<std::shared_ptr<Node>> declarations();
  std::vector<std::shared_ptr<Node>> var_declaration();
  std::shared_ptr<Node> type();

  std::shared_ptr<Node> parse();
};
