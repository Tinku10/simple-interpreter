#include "CallStack.h"

ActivationRecords::ActivationRecords(const std::string& name,
                                     ActivationRecordType type,
                                     uint level)
    : name(name), type(type), level(level) {
}

void ActivationRecords::add(std::string& name, std::shared_ptr<DataType> value) {
  members[name] = value;
}

std::shared_ptr<DataType> ActivationRecords::at(std::string& name) {
  /* if (members.count(name)) return members.at(name); */

  /* throw error(ErrorCode::IDENTIFIER_NOT_FOUND); */
}

std::ostream& operator<<(std::ostream& cout, ActivationRecords& stack_frame) {
  std::cout << "--------------------------\n";
  std::cout << stack_frame.level << ".\t" << stack_frame.name << "\n";

  std::cout << "members\n-------\n";

  for (auto& e : stack_frame.members) {
    cout << e.first << "\t" << e.second << "\n";
  }
  std::cout << "--------------------------\n";

  return cout;
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

std::shared_ptr<DataType> CallStack::at(std::string& name) {
  /* std::cout << "resolving " << name << " at " */
  /*           << " " << top()->name << "\n"; */
  /* std::cout << *top() << "\n"; */

  if (top()->members.count(name)) return top()->members[name];

  std::unique_ptr<ActivationRecords> stack_frame = std::move(top());

  if (stack.empty()) throw error(ErrorCode::IDENTIFIER_NOT_FOUND);

  pop();
  std::shared_ptr<DataType> value = at(name);
  push(std::move(stack_frame));

  return value;
}

std::ostream& operator<<(std::ostream& cout, CallStack& callstack) {
  if (callstack.stack.empty()) return cout;
  /* while(!callstack.stack.empty()) { */
  std::unique_ptr<ActivationRecords> stack_frame = std::move(callstack.top());

  cout << *stack_frame << "\n";

  callstack.pop();
  cout << callstack << "\n";
  callstack.push(std::move(stack_frame));
  /* } */
  return cout;
}

Exception CallStack::error(ErrorCode code) {
  std::string msg = "";
  switch (code) {
    case ErrorCode::IDENTIFIER_NOT_FOUND:
      msg = "undeclared identifier\n";
      break;
    default: msg = "unknown error\n";
  }

  throw Exception(code, msg);
}
