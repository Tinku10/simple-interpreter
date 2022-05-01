#include <iostream>

class Symbol {
public:
  std::string name;
  std::shared_ptr<Symbol> type;

  Symbol(std::string& name, std::shared_ptr<Symbol> type = nullptr);
};

class BuiltInTypeSymbol : public Symbol {
public:
  BuiltInTypeSymbol(std::string& name);

  friend std::ostream& operator<<(std::ostream& cout, BuiltInTypeSymbol& symbol);
};

class VarTypeSymbol : public Symbol {
public:
  VarTypeSymbol(std::string& name, std::shared_ptr<Symbol> type);

  friend std::ostream& operator<<(std::ostream& cout, VarTypeSymbol& symbol);
};
