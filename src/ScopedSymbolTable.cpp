#include "ScopedSymbolTable.h"

#include <vector>

ScopedSymbolTable::ScopedSymbolTable(std::string&& scope_name, uint scope_level, std::shared_ptr<ScopedSymbolTable> parent_scope)
    : scope_name(scope_name), scope_level(scope_level), parent_scope(parent_scope) {
  initialize();
}

void ScopedSymbolTable::initialize() {
  std::string types[] = {"INTEGER", "REAL"};

  for (int i = 0; i < 2; i++) {
    symbols[types[i]]
        = std::make_shared<BuiltInTypeSymbol>(BuiltInTypeSymbol(types[i]));
  }
}

std::shared_ptr<Symbol> ScopedSymbolTable::at(std::string& name) {
  std::shared_ptr<ScopedSymbolTable> scope = std::make_shared<ScopedSymbolTable>(*this);

  while(scope) {
    std::cout << name << " lookup at " << scope->scope_name << " scope\n";
    if (scope->symbols.count(name)) return scope->symbols.at(name);
    scope = scope->parent_scope;
  }

  /* if (symbols.count(name)) return symbols.at(name); */
  return nullptr;
}

std::ostream& operator<<(std::ostream& cout, ScopedSymbolTable& symbols) {
  for (auto& e : symbols.symbols) {
    std::shared_ptr<VarTypeSymbol> symbol
        = std::dynamic_pointer_cast<VarTypeSymbol>(e.second);
    if (symbol == nullptr) {
      std::shared_ptr<ProcedureSymbol> symbol
          = std::dynamic_pointer_cast<ProcedureSymbol>(e.second);
      if (symbol == nullptr) {
        std::shared_ptr<BuiltInTypeSymbol> symbol
            = std::dynamic_pointer_cast<BuiltInTypeSymbol>(e.second);
        cout << e.first << " : " << *(symbol) << "\n";
      } else {
        cout << e.first << " : " << *(symbol) << "\n";
      }
    } else {
      cout << e.first << " : " << *(symbol) << "\n";
    }
  }

  return cout;
}
