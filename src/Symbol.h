#include <iostream>
#include <vector>

class Symbol {
 public:
  std::string name;
  std::shared_ptr<Symbol> type;

  Symbol(std::string& name, std::shared_ptr<Symbol> type = nullptr);

  virtual ~Symbol() = 0;

  friend std::ostream& operator<<(std::ostream& cout, Symbol& symbol);
};

class BuiltInTypeSymbol : public Symbol {
 public:
  BuiltInTypeSymbol(std::string& name);
  ~BuiltInTypeSymbol() override;

  friend std::ostream& operator<<(std::ostream& cout,
                                  BuiltInTypeSymbol& symbol);
};

class VarTypeSymbol : public Symbol {
 public:
  VarTypeSymbol(std::string& name, std::shared_ptr<Symbol> type);
  ~VarTypeSymbol() override;

  friend std::ostream& operator<<(std::ostream& cout, VarTypeSymbol& symbol);
};

class ProcedureSymbol : public Symbol {
 public:
  std::vector<std::shared_ptr<Symbol>> declarations;

  ProcedureSymbol(std::string& name,
                  std::vector<std::shared_ptr<Symbol>>& declarations);
  ~ProcedureSymbol() override;

  friend std::ostream& operator<<(std::ostream& cout, ProcedureSymbol& symbol);
};
