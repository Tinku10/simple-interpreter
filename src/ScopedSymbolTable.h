#include <map>
#include <variant>

#include "Symbol.h"

class SymbolWithScope {
 public:
  std::shared_ptr<Symbol> symbol;
  std::string scope_name;
  uint scope_level;

  SymbolWithScope(std::shared_ptr<Symbol> symbol,
                  std::string& scope_name,
                  uint scope_level);

  virtual ~SymbolWithScope();
};

class ScopedSymbolTable {
 public:
  std::map<std::string, std::shared_ptr<SymbolWithScope>> symbols;
  std::string scope_name;
  uint scope_level;
  std::shared_ptr<ScopedSymbolTable> parent_scope;

  ScopedSymbolTable(std::string&& scope_name,
                    uint scope_level,
                    std::shared_ptr<ScopedSymbolTable> parent_scope = nullptr);

  void initialize();

  template <typename T>
  void add(T symbol) {
    /* std::cout << "Insert symbol " << symbol->name << "\n"; */
    symbol->scope_level = scope_level;
    symbols[symbol->name] = std::make_shared<SymbolWithScope>(
        SymbolWithScope(symbol, scope_name, scope_level));
  }

  std::shared_ptr<SymbolWithScope> at(std::string& name, uint args_size = 0, 
                                      uint max_scope_jump = INT_MAX);

  friend std::ostream& operator<<(std::ostream& cout,
                                  ScopedSymbolTable& symbols);
};
