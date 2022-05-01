#include "SymbolTable.h"

SymbolTable::SymbolTable() {
  initialize();
}

void SymbolTable::initialize() {
  std::string types[] = {"INTEGER", "REAL"};

  for(int i = 0; i < 2; i++) {
    symbols[types[i]] = std::make_shared<BuiltInTypeSymbol>(BuiltInTypeSymbol(types[i]));
  }
}

std::shared_ptr<Symbol> SymbolTable::at(std::string& name) {
  if(symbols.count(name)) return symbols[name];

  std::string mesg = name + " not declared";
  throw std::invalid_argument(mesg);
}

void SymbolTable::add(std::shared_ptr<Symbol> symbol) {
  std::shared_ptr<VarTypeSymbol> s = std::static_pointer_cast<VarTypeSymbol>(symbol);
  symbols[s->name] = symbol;
}

std::ostream& operator<<(std::ostream& cout, SymbolTable& symbols) {
  for(auto& e: symbols.symbols) {
    cout << "(" << e.first << " : " << e.second << ") ";
  }

  return cout;
}
