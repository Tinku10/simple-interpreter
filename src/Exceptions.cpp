#include "Exceptions.h"

Exception::Exception(ErrorCode code, std::string& message)
    : code(code), message(message) {
}

const char* Exception::what() const noexcept {
  return message.c_str();
}
