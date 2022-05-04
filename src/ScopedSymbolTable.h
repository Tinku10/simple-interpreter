#include <map>
#include "Symbol.h"
#include <variant>

class ScopedSymbolTable {
public:
  std::map<std::string, std::shared_ptr<Symbol>> symbols;
  std::string scope_name;
  uint scope_level;

  ScopedSymbolTable(std::string&& scope_name, uint scope_level);

  void initialize();

  template <typename T>
  void add(T symbol) {
    symbols[symbol->name] = symbol;
  }

  std::shared_ptr<Symbol> at(std::string& name);

  friend std::ostream& operator<<(std::ostream& cout, ScopedSymbolTable& symbols);
};
