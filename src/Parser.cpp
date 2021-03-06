#include "Parser.h"

Parser::Parser(Lexer& lexer) : lexer(lexer) {
}

void Parser::eat(TokenType type) {
  if (current_token.type != type) {
    throw error(ErrorCode::UNEXPECTED_TOKEN);
    /* throw std::invalid_argument("Unexpected token"); */
  }
  current_token = lexer.get_next_token();
}

std::shared_ptr<Node> Parser::factor() {
  if (current_token.type == TokenType::INTEGER_CONST
      || current_token.type == TokenType::REAL_CONST
      || current_token.type == TokenType::STRING_CONST
      || current_token.type == TokenType::TRUE
      || current_token.type == TokenType::FALSE) {
    Token token = current_token;
    eat(current_token.type);
    return std::make_shared<LiteralNode>(LiteralNode(token));
  }

  if (current_token.type == TokenType::ID) {
    Token token = current_token;
    eat(TokenType::ID);
    return std::make_shared<VarNode>(VarNode(token));
  }

  if (current_token.type == TokenType::PLUS
      || current_token.type == TokenType::MINUS) {
    Token token = current_token;
    eat(current_token.type);
    return std::make_shared<UnaryNode>(UnaryNode(factor(), token));
  }

  if (current_token.type == TokenType::L_PAREN) {
    eat(TokenType::L_PAREN);
    std::shared_ptr<Node> node = rel_expr();
    eat(TokenType::R_PAREN);

    return node;
  }

  return std::make_shared<NoOpNode>(NoOpNode());
}

std::shared_ptr<Node> Parser::term() {
  std::shared_ptr<Node> left = factor();

  while (current_token.type == TokenType::MULTIPLY
         || current_token.type == TokenType::FLOAT_DIV
         || current_token.type == TokenType::INT_DIV) {
    Token token = current_token;
    eat(current_token.type);
    left = std::make_shared<BinaryNode>(BinaryNode(left, token, factor()));
  }

  return left;
}

std::shared_ptr<Node> Parser::expr() {
  std::shared_ptr<Node> left = term();

  while (current_token.type == TokenType::PLUS
         || current_token.type == TokenType::MINUS) {
    Token token = current_token;
    eat(current_token.type);
    left = std::make_shared<BinaryNode>(BinaryNode(left, token, term()));
  }

  return left;
}

std::shared_ptr<Node> Parser::rel_expr() {
  std::shared_ptr<Node> left = expr();

  while (current_token.type == TokenType::LESS_THAN
           || current_token.type == TokenType::GREATER_THAN
           || current_token.type == TokenType::LT_EQUAL
           || current_token.type == TokenType::GT_EQUAL
           || current_token.type == TokenType::EQUAL_EQUAL
           || current_token.type == TokenType::NOT_EQUAL) {
      Token token = current_token;
      eat(current_token.type);
      left = std::make_shared<BinaryNode>(left, token, expr());

  }

  return left;
}

std::shared_ptr<Node> Parser::statement() {
  if (current_token.type == TokenType::BEGIN) {
    return compound();
  }

  if (current_token.type == TokenType::ID && lexer.source[lexer.index] == '(') {
    return procedure_call_statement();
  }

  if (current_token.type == TokenType::IF) {
    return if_statement();
  }

  if (current_token.type == TokenType::ID) {
    std::shared_ptr<Node> left = factor();

    Token token = current_token;
    eat(TokenType::EQUAL);

    return std::make_shared<AssignNode>(AssignNode(left, token, rel_expr()));
  }

  return std::make_shared<NoOpNode>(NoOpNode());
}

std::shared_ptr<Node> Parser::if_statement() {
  std::vector<std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>> v;

  Token token = current_token;

  eat(TokenType::IF);

  std::shared_ptr<Node> expr = rel_expr();

  eat(TokenType::THEN);

  v.emplace_back(std::make_pair(expr, compound()));

  std::cout << current_token;
  if (current_token.type == TokenType::ELSE) {
    eat(current_token.type);
    v.emplace_back(std::make_pair(nullptr, compound()));
  }

  return std::make_shared<IfStatementNode>(IfStatementNode(token, v));
}

std::shared_ptr<Node> Parser::procedure_call_statement() {
  Token token = current_token;

  std::string name = token.value;

  std::shared_ptr<Node> id = factor();

  eat(TokenType::L_PAREN);

  std::vector<std::shared_ptr<Node>> v;

  if (current_token.type != TokenType::R_PAREN) {
    v.emplace_back(rel_expr());

    while (current_token.type == TokenType::COMMA) {
      eat(current_token.type);
      v.emplace_back(rel_expr());
    }
  }

  eat(TokenType::R_PAREN);

  return std::make_shared<ProcedureCallNode>(ProcedureCallNode(token, name, v));
}

std::shared_ptr<Node> Parser::statement_list() {
  std::vector<std::shared_ptr<Node>> v;
  v.push_back(statement());

  while (current_token.type == TokenType::SEMI) {
    eat(current_token.type);
    v.push_back(statement());
  }

  return std::make_shared<CompoundNode>(CompoundNode(v));
}

std::shared_ptr<Node> Parser::compound() {
  eat(TokenType::BEGIN);
  std::shared_ptr<Node> node = statement_list();
  eat(TokenType::END);

  return node;
}

std::shared_ptr<Node> Parser::type() {
  Token token = current_token;
  if (current_token.type == TokenType::INTEGER) {
    eat(TokenType::INTEGER);
  } else if (current_token.type == TokenType::REAL) {
    eat(TokenType::REAL);
  } else if (current_token.type == TokenType::STRING) {
    eat(TokenType::STRING);
  } else if (current_token.type == TokenType::BOOLEAN) {
    eat(TokenType::BOOLEAN);
  } else {
    throw error(ErrorCode::UNEXPECTED_TOKEN);
    /* std::invalid_argument("Invalid type"); */
  }
  return std::make_shared<TypeNode>(TypeNode(token));
}

std::vector<std::shared_ptr<Node>> Parser::var_declaration() {
  std::vector<std::shared_ptr<Node>> v;

  v.emplace_back(factor());

  while (current_token.type == TokenType::COMMA) {
    eat(current_token.type);
    v.emplace_back(factor());
  }

  eat(TokenType::COLON);

  std::shared_ptr<Node> node = type();

  std::vector<std::shared_ptr<Node>> declarations;

  for (auto& child : v) {
    declarations.emplace_back(
        std::make_shared<VarDeclNode>(VarDeclNode(child, node)));
  }

  return declarations;
}

std::vector<std::shared_ptr<Node>> Parser::parameters() {
  std::vector<std::shared_ptr<Node>> v;

  v.emplace_back(factor());

  while (current_token.type == TokenType::COMMA) {
    eat(current_token.type);
    v.emplace_back(factor());
  }

  eat(TokenType::COLON);

  std::shared_ptr<Node> node = type();

  std::vector<std::shared_ptr<Node>> list;

  for (auto& child : v) {
    std::shared_ptr<VarNode> var = std::static_pointer_cast<VarNode>(child);
    list.emplace_back(std::make_shared<ParamsNode>(ParamsNode(var, node)));
  }

  return list;
}

std::vector<std::shared_ptr<Node>> Parser::parameters_list() {
  std::vector<std::shared_ptr<Node>> v;

  std::vector<std::shared_ptr<Node>> list = parameters();
  for (auto& child : list) {
    v.emplace_back(child);
  }

  while (current_token.type == TokenType::SEMI) {
    eat(current_token.type);
    list = parameters();
    for (auto& child : list) {
      v.emplace_back(child);
    }
  }

  return v;
}

std::vector<std::shared_ptr<Node>> Parser::declarations() {
  std::vector<std::shared_ptr<Node>> v;

  while (current_token.type == TokenType::VAR) {
    eat(TokenType::VAR);

    while (current_token.type == TokenType::ID) {
      std::vector<std::shared_ptr<Node>> declarations = var_declaration();
      eat(TokenType::SEMI);

      for (auto& child : declarations) v.push_back(child);
    }
  }

  while (current_token.type == TokenType::PROCEDURE) {
    eat(TokenType::PROCEDURE);
    std::shared_ptr<VarNode> id = std::static_pointer_cast<VarNode>(factor());

    std::vector<std::shared_ptr<Node>> parameters;
    if (current_token.type == TokenType::L_PAREN) {
      eat(TokenType::L_PAREN);
      parameters = parameters_list();
      eat(TokenType::R_PAREN);
    }
    eat(TokenType::SEMI);

    std::shared_ptr<Node> b = block();
    eat(TokenType::SEMI);

    v.emplace_back(std::make_shared<ProcedureDeclNode>(
        ProcedureDeclNode(id->token.value, parameters, b)));
  }

  return v;
}

std::shared_ptr<Node> Parser::block() {
  std::vector<std::shared_ptr<Node>> list = declarations();

  return std::make_shared<BlockNode>(BlockNode(list, compound()));
}

std::shared_ptr<Node> Parser::program() {
  eat(TokenType::PROGRAM);
  std::shared_ptr<Node> var = factor();
  eat(TokenType::SEMI);
  std::shared_ptr<Node> node = block();
  eat(TokenType::DOT);
  eat(TokenType::ENDOFFILE);

  return std::make_shared<ProgramNode>(ProgramNode(var, node));
}

std::shared_ptr<Node> Parser::parse() {
  current_token = lexer.get_next_token();
  return program();
}

Exception Parser::error(ErrorCode code) {
  std::string msg = "";
  switch (code) {
    case ErrorCode::UNEXPECTED_TOKEN: msg = "unexpected token\n"; break;
    default: msg = "unknown error\n";
  }

  msg += "syntax error at line " + std::to_string(this->current_token.line)
         + " column " + std::to_string(this->current_token.start);
  throw Exception(code, msg);
}
