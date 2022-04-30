#pragma once

enum struct TokenType {
  L_PAREN,
  R_PAREN,
  L_BRACE,
  R_BRACE,
  EQUAL,
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
  LESS_THAN,
  GREATER_THAN,
  ENDOFFILE,
  SEMI,

  NOT_EQUAL,
  GT_EQUAL,
  LT_EQUAL,
  EQUAL_EQUAL,

  ID,
  NUMBER,
  STRING,

  IF,
  ELSE,
  ELSE_IF,
  FOR,
  WHILE,

  BEGIN,
  END,
  DOT
};
