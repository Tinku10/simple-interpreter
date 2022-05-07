#include "ScopedSymbolTable.h"

#include <vector>

SymbolWithScope::SymbolWithScope(std::shared_ptr<Symbol> symbol,
                                 std::string& scope_name,
                                 uint scope_level)
    : symbol(symbol), scope_name(scope_name), scope_level(scope_level) {
}

SymbolWithScope::~SymbolWithScope() {
}

ScopedSymbolTable::ScopedSymbolTable(
    std::string&& scope_name,
    uint scope_level,
    std::shared_ptr<ScopedSymbolTable> parent_scope)
    : scope_name(scope_name),
      scope_level(scope_level),
      parent_scope(parent_scope) {
  // insert INTEGER and REAL only for builtin scope
  if (scope_level == 0) initialize();
}

void ScopedSymbolTable::initialize() {
  std::string types[] = {"INTEGER", "REAL"};

  for (int i = 0; i < 2; i++) {
    add(std::make_shared<BuiltInTypeSymbol>(BuiltInTypeSymbol(types[i])));
  }
}

std::shared_ptr<SymbolWithScope> ScopedSymbolTable::at(std::string& name,
                                                       uint args_size,
                                                       uint max_scope_jump) {
  std::shared_ptr<ScopedSymbolTable> scope
      = std::make_shared<ScopedSymbolTable>(*this);

  while (scope && max_scope_jump--) {
    /* std::cout << name << " lookup at " << scope->scope_name << " scope\n"; */
    if (scope->symbols.count(name)) {
      std::shared_ptr<SymbolWithScope> symbol = scope->symbols.at(name);

      std::shared_ptr<ProcedureSymbol> procedure_symbol
          = std::dynamic_pointer_cast<ProcedureSymbol>(symbol->symbol);

      if (procedure_symbol) {
        if (procedure_symbol->declarations.size() == args_size) return symbol;

        return nullptr;
      }
      if (symbol->symbol) return scope->symbols.at(name);
    }

    scope = scope->parent_scope;
  }

  /* if (symbols.count(name)) return symbols.at(name); */
  return nullptr;
}

std::ostream& operator<<(std::ostream& cout, ScopedSymbolTable& symbols) {
  for (auto& e : symbols.symbols) {
    std::shared_ptr<VarTypeSymbol> symbol
        = std::dynamic_pointer_cast<VarTypeSymbol>(e.second->symbol);
    if (symbol == nullptr) {
      std::shared_ptr<ProcedureSymbol> symbol
          = std::dynamic_pointer_cast<ProcedureSymbol>(e.second->symbol);
      if (symbol == nullptr) {
        std::shared_ptr<BuiltInTypeSymbol> symbol
            = std::dynamic_pointer_cast<BuiltInTypeSymbol>(e.second->symbol);
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
