#include <map>
#include "Symbol.h"

class SymbolTable {
public:
  std::map<std::string, std::shared_ptr<Symbol>> symbols;

  SymbolTable();

  void initialize();

  void add(std::shared_ptr<Symbol> symbol);

  std::shared_ptr<Symbol> at(std::string& name);

  friend std::ostream& operator<<(std::ostream& cout, SymbolTable& symbols);
};
