#include "Symbol.h"

#include <vector>

Symbol::Symbol(std::string& name, std::shared_ptr<Symbol> type)
    : name(name), type(type) {
}

Symbol::~Symbol() {
}

std::ostream& operator<<(std::ostream& cout, Symbol& node) {
  if (node.type != nullptr)
    cout << node.type->name;
  else
    cout << node.name;
  return cout;
}

BuiltInTypeSymbol::BuiltInTypeSymbol(std::string& name) : Symbol(name) {
}

BuiltInTypeSymbol::~BuiltInTypeSymbol() {
}

std::ostream& operator<<(std::ostream& cout, BuiltInTypeSymbol& node) {
  cout << "<BuiltInTypeSymbol(name='" << node.name << "')>";
  return cout;
}

VarTypeSymbol::VarTypeSymbol(std::string& name, std::shared_ptr<Symbol> type)
    : Symbol(name, type) {
}

VarTypeSymbol::~VarTypeSymbol() {
}

std::ostream& operator<<(std::ostream& cout, VarTypeSymbol& node) {
  cout << "<VarTypeSymbol(name='" << node.name << "', type='" << node.type->name
       << "')>";
  return cout;
}

ProcedureSymbol::ProcedureSymbol(
    std::string& name,
    std::vector<std::shared_ptr<Symbol>>& declarations)
    : Symbol(name), declarations(declarations) {
}

ProcedureSymbol::~ProcedureSymbol() {
}

std::ostream& operator<<(std::ostream& cout, ProcedureSymbol& node) {
  cout << "<ProcedureSymbol(name='" << node.name << "'), ";
  cout << "parameters=[";
  for (auto& p : node.declarations) {
    cout << "<VarTypeSymbol(name='" << p->name << "', type='" << p->type->name
         << "')>";
  }
  cout << "]";
  return cout;
}
