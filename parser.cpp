#include <iostream>
#include <set>
#include "token.hpp"
#include "node.hpp"

auto parse(std::vector<Token> tkns) -> Program * {
    auto result  = new Program();
    auto current = tkns.begin();

    while (current->kind != Kind::EndOfToken) {
        switch(current->kind) {
        case Kind::Function: 
            result->funcs.push_back(parseFunction(current));
            break;
        default:
            std::cout << "Invalid statement : " << current->str << std::endl;
        }
    }

    return result;
}

auto parseFunction(std::vector<Token>::iterator & current) -> Function * {
    auto result = new Function();

    skipCurrent(current, Kind::Function);

    result->name = current->str;

    skipCurrent(current, Kind::Identifier);
    skipCurrent(current, Kind::LeftParen);

    if (current->kind != Kind::RightParen) {
        do {
            result->params.push_back(current->str);
            skipCurrent(current, Kind::Identifier);
        } while(skipCurrentIf(current, Kind::Comma));
    }

    skipCurrent(current, Kind::RightParen);

    result->block = parseBlock(current);

    return result;
}

auto parseBlock(std::vector<Token>::iterator & current) -> std::vector<Statement *> {
    using std::vector;

    auto result = vector<Statement *>();

    skipCurrent(current, Kind::LeftBrace);

    while (current->kind != Kind::RightBrace) {
        switch (current->kind) {
        case Kind::Variable:
            result.push_back(parseVariable(current));
            break;
        default: 
        result.push_back(parseExpressionStatement(current));
        }
    }

    skipCurrent(current, Kind::RightBrace);

    return result;
}

auto parseExpressionStatement(std::vector<Token>::iterator & current) -> ExpressionStatement * {
    auto result = new ExpressionStatement((ExpressionStatement) {.exp = parseExpression(current)});

    skipCurrent(current, Kind::Semicolon);

    return result;
}

auto parseVariable(std::vector<Token>::iterator & current) -> Variable * {
    auto result = new Variable();

    skipCurrent(current, Kind::Variable);

    result->name = current->str;

    skipCurrent(current, Kind::Identifier);
    skipCurrent(current, Kind::Assignment);

    result->exp = parseExpression(current);

    skipCurrent(current, Kind::Semicolon);

    return result;
}

auto parseExpression(std::vector<Token>::iterator & current) -> Expression * {
    return parseAssignment(current);
}

auto parseAssignment(std::vector<Token>::iterator & current) -> Expression * {
    auto result = parseOr(current);

    if (current->kind != Kind::Assignment) return result;

    skipCurrent(current, Kind::Assignment);

    if (auto get_var = dynamic_cast<GetVariable *>(result)) {
        return new SetVariable((SetVariable) {
            .name = get_var.name, 
            .val  = parseAssignment(current),
        });
    }

    if (auto get_elem = dynamic_cast<GetElement *>(result)) {
        return new SetElement((SetElement) {
            .sub = get_elem.sub,
            .idx = get_elem.idx,
            .val = parseAssignment(current),
        });
    }

    std::cout << "wrong assignment expression\n";
    exit(1);
}

auto parseOr(std::vector<Token>::iterator & current) -> Expression * {
    auto result = parseAnd(current);

    while (skipCurrentIf(current, Kind::LogicalOr)) {
        result = new Or((Or) {
            .lhs = result,
            .rhs = parseAnd(current),
        });
    }

    return result;
}

auto parseAnd(std::vector<Token>::iterator & current) -> Expression * {
    auto result = parseRelational(current);

    while (skipCurrentIf(current, Kind::LogicalAnd)) {
        result = new And((And) {
            .lhs = result,
            .rhs = parseRelational(current),
        });
    }

    return result;
}

auto parseRelational(std::vector<Token>::iterator & current) -> Expression * {
    using std::set;
    
    set<Kind> operators = {
        Kind::Equal,       Kind::NotEqual,
        Kind::LessThan,    Kind::GreaterThan,
        Kind::LessOrEqual, Kind::GreaterOrEqual,
    };

    auto result = parseArithmetic1(current);

    while (operators.count(current->kind)) {
        skipCurrent(current);

        result = new Relational((Relational) {
            .kind = current->kind,
            .lhs  = result,
            .rhs  = parseArithmetic1(current),
        });
    }

    return result;
}

auto parseArithmetic1(std::vector<Token>::iterator & current) -> Expression * {
    using std::set;

    set<Kind> operators = { Kind::Add, Kind::Subtract };
    
    auto result = parseArithmetic2(current);
    
    while (operators.count(current->kind)) {
        skipCurrent(current);

        result = new Arithmetic((Arithmetic) {
            .kind = current->kind,
            .lhs  = result,
            .rhs  = parseArithmetic2(current),
        });
    }

    return result;
}

auto parseArithmetic2(std::vector<Token>::iterator & current) -> Expression * {
    using std::set;

    set<Kind> operators = { Kind::Multiply, Kind::Divide, Kind::Modulo };
    
    auto result = parseUnary(current);
    
    while (operators.count(current->kind)) {
        result = new Arithmetic((Arithmetic) {
            .kind = current->kind,
            .lhs  = result,
            .rhs  = parseUnary(current),
        });
    }

    return result;
}

auto parseUnary(std::vector<Token>::iterator & current) -> Expression * {
    using std::set;
    
    set<Kind> operators = { Kind::Add, Kind::Subtract };
    
    return !operators.count(current->kind)
        ? parseOperand(current)
        : new Unary((Unary) {
            .kind = current->kind,
            .sub  = parseUnary(current)
        });
}

auto parseOperand(std::vector<Token>::iterator & current) -> Expression * {
  Expression * result = nullptr;
  switch (current->kind) {
  case Kind::NullLiteral:   result = parseNullLiteral();      break;
  case Kind::TrueLiteral:
  case Kind::FalseLiteral:  result = parseBooleanLiteral();   break;
  case Kind::NumberLiteral: result = parseNumberLiteral();    break;
  case Kind::StringLiteral: result = parseStringLiteral();    break;
  case Kind::LeftBraket:    result = parseListLiteral();      break;
  case Kind::LeftBrace:     result = parseMapLiteral();       break;
  case Kind::Identifier:    result = parseIdentifier();       break;
  case Kind::LeftParen:     result = parseInnerExpression();  break;
  default:                  cout << "잘못된 식입니다.";       exit(1);
  }
  return parsePostfix(result);
}

auto parseNullLiteral(std::vector<Token>::iterator & current) -> Expression * {
  skipCurrent(Kind::NullLiteral);
  auto result = new NullLiteral();
  return result;
}

auto parseBooleanLiteral(std::vector<Token>::iterator & current) -> Expression * {
  auto result = new BooleanLiteral();
  result->value = current->kind == Kind::TrueLiteral;
  skipCurrent();
  return result;
}

auto parseNumberLiteral(std::vector<Token>::iterator & current) -> Expression * {
  auto result = new NumberLiteral();
  result->value = stod(current->string);
  skipCurrent(Kind::NumberLiteral);
  return result;
}

auto parseStringLiteral(std::vector<Token>::iterator & current) -> Expression * {
  auto result = new StringLiteral();
  result->value = current->string;
  skipCurrent(Kind::StringLiteral);
  return result;
}

auto parseListLiteral(std::vector<Token>::iterator & current) -> Expression * {
  auto result = new ArrayLiteral();
  skipCurrent(Kind::LeftBraket);
  if (current->kind != Kind::RightBraket) {
    do result->values.push_back(parseExpression());
    while (skipCurrentIf(Kind::Comma));
  }
  skipCurrent(Kind::RightBraket);
  return result;
}

auto parseMapLiteral(std::vector<Token>::iterator & current) -> Expression * {
  auto result = new MapLiteral();
  skipCurrent(Kind::LeftBrace);
  if (current->kind != Kind::RightBrace) {
    do {
      auto name = current->string;
      skipCurrent(Kind::StringLiteral);
      skipCurrent(Kind::Colon);
      auto value = parseExpression();
      result->values[name] = value;
    } while (skipCurrentIf(Kind::Comma));
  }
  skipCurrent(Kind::RightBrace);
  return result;
}

auto parseIdentifier(std::vector<Token>::iterator & current) -> Expression * {
  auto result = new GetVariable();
  result->name = current->string;
  skipCurrent(Kind::Identifier);
  return result;
}

auto parseInnerExpression(std::vector<Token>::iterator & current) -> Expression * {
  skipCurrent(Kind::LeftParen);
  auto result = parseExpression();
  skipCurrent(Kind::RightParen);
  return result;
}

auto parsePostfix(Expression * sub)->Expression * {
  while (true) {
    switch (current->kind) {
    case Kind::LeftParen:  sub = parseCall(sub);    break;
    case Kind::LeftBraket: sub = parseElement(sub); break;
    default: return sub;
    }
  }
}

auto parseCall(Expression * sub)->Expression * {
  auto result = new Call();
  result->sub = sub;
  skipCurrent(Kind::LeftParen);
  if (current->kind != Kind::RightParen) {
    do result->arguments.push_back(parseExpression());
    while (skipCurrentIf(Kind::Comma));
  }
  skipCurrent(Kind::RightParen);
  return result;
}

auto parseElement(std::vector<Token>::iterator & current, Expression * sub)->Expression * {
  skipCurrent(current, Kind::LeftBraket);

  auto result = new GetElement((GetElement) {
    .sub = sub,
    .idx = parseExpression(current),
  });

  skipCurrent(current, Kind::RightBraket);
  
  return result;
}

auto skipCurrent(std::vector<Token>::iterator & current) -> void {
    current++;
}

auto skipCurrent(std::vector<Token>::iterator & current, Kind kind) -> void {
    if ((*current++).kind != kind) {
        std::cout << "The token does not found : " << toString(kind) << std::endl;

        exit(1);
    }
}

auto skipCurrentIf(std::vector<Token>::iterator & current, Kind kind) -> bool {
    return (current += current->kind == kind) - current;
}