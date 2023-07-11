#include <iostream>
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

auto skipCurrent(std::vector<Token>::iterator & current, Kind kind) -> void {
    if ((*current++).kind != kind) {
        std::cout << "The token does not found : " << toString(kind) << std::endl;

        exit(1);
    }
}

auto skipCurrentIf(std::vector<Token>::iterator & current, Kind kind) -> bool {
    return (current += current->kind == kind) - current;
}