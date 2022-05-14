#pragma once

#include <iostream>
#include <map>
#include <stack>

#include "Exceptions.h"
#include "Token.h"
#include "DataType.h"

enum struct ActivationRecordType { PROGRAM, PROCEDURE };

class ActivationRecords {
 public:
  std::string name;
  ActivationRecordType type;
  uint level;
  std::map<std::string, std::shared_ptr<DataType>> members;

  ActivationRecords(const std::string& name,
                    ActivationRecordType type,
                    uint level);

  void add(std::string& name, std::shared_ptr<DataType> value);

  std::shared_ptr<DataType> at(std::string& name);

  Exception error(ErrorCode code);

  friend std::ostream& operator<<(std::ostream& cout,
                                  ActivationRecords& stack_frame);
};

class CallStack {
 public:
  std::map<std::string, std::shared_ptr<DataType>> cache;
  std::stack<std::unique_ptr<ActivationRecords>> stack;

  CallStack();

  void push(std::unique_ptr<ActivationRecords> stack_frame);
  void pop();
  std::unique_ptr<ActivationRecords>& top();

  std::shared_ptr<DataType> at(std::string& name);

  Exception error(ErrorCode code);

  friend std::ostream& operator<<(std::ostream& cout, CallStack& callstack);
};
