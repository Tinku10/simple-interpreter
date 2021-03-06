#include <iostream>
#include <vector>

#include "TokenType.h"

class Node;

class Symbol {
 public:
  std::string name;
  std::shared_ptr<Symbol> type;
  uint scope_level;

  Symbol(std::string& name, std::shared_ptr<Symbol> type = nullptr);

  virtual ~Symbol() = 0;

  friend std::ostream& operator<<(std::ostream& cout, Symbol& symbol);
};

class BuiltInTypeSymbol : public Symbol {
 public:
  TokenType datatype;

  BuiltInTypeSymbol(std::string& name, TokenType datatype);
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
  std::shared_ptr<Node> block;

  ProcedureSymbol(std::string& name,
                  std::vector<std::shared_ptr<Symbol>>& declarations);
  ~ProcedureSymbol() override;

  friend std::ostream& operator<<(std::ostream& cout, ProcedureSymbol& symbol);
};
