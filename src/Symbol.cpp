#include "Symbol.h"

Symbol::Symbol(std::string& name, std::shared_ptr<Symbol> type)
    : name(name), type(type) {
}

BuiltInTypeSymbol::BuiltInTypeSymbol(std::string& name) : Symbol(name) {
}

VarTypeSymbol::VarTypeSymbol(std::string& name, std::shared_ptr<Symbol> type)
    : Symbol(name, type) {
}
