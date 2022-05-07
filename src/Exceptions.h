#pragma once

#include <exception>
#include <iostream>
/* #include "Token.h" */

enum struct ErrorCode {
  UNEXPECTED_TOKEN,
  DUPLICATE_IDENTIFIER,
  IDENTIFIER_NOT_FOUND,
  UNKNOWN_CHAR,
  PEEK_PAST_LENGTH
};

class Exception : public std::exception {
 public:
  ErrorCode code;
  std::string message;

  Exception(ErrorCode code, std::string& message);

  const char* what() const noexcept override;
};

class LexerException : public Exception {};

class ParserException : public Exception {};

class SymanticsException : public Exception {};
