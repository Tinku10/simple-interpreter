#include "CallStack.h"

ActivationRecords::ActivationRecords(const std::string& name,
                                     ActivationRecordType type,
                                     uint level)
    : name(name), type(type), level(level) {
}

void ActivationRecords::add(std::string& name, int value) {
  members[name] = value;
}

int ActivationRecords::at(std::string& name) {
  if (members.count(name)) return members.at(name);

  throw error(ErrorCode::IDENTIFIER_NOT_FOUND);
}

Exception ActivationRecords::error(ErrorCode code) {
  std::string msg = "";
  switch (code) {
    case ErrorCode::IDENTIFIER_NOT_FOUND:
      msg = "undeclared identifier\n";
      break;
    default: msg = "unknown error\n";
  }

  throw Exception(code, msg);
}

CallStack::CallStack() {
}

void CallStack::push(std::unique_ptr<ActivationRecords> stack_frame) {
  stack.push(std::move(stack_frame));
}

void CallStack::pop() {
  stack.pop();
}

std::unique_ptr<ActivationRecords>& CallStack::top() {
  return stack.top();
}
