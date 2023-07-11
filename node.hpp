#pragma once

#include <vector>
#include <string>
#include <map>
#include "kind.hpp"

struct Program {
    std::vector<Function *> funcs;
};

struct Statement {};

struct Expression {};

struct Function: Statement {
    std::string              name;
    std::vector<std::string> params;
    std::vector<Statement *> block;
};

struct Return: Statement {
    Expression * exp;
};

struct Variable: Statement {
    std::string name;
    Expression  * exp;
};

struct For: Statement {
    Variable                 * var;
    Expression               * cond;
    Expression               * update;
    std::vector<Statement *> block;
};

struct Break: Statement {};

struct Continue: Statement {};

struct If: Statement {
    std::vector<Expression *>             conds;
    std::vector<std::vector<Statement *>> t_blocks;
    std::vector<Statement *>              e_block;
};

struct Print: Statement {
    bool                      line_feed = false;
    std::vector<Expression *> args;
};

struct ExpressionStatement: Statement {
    Expression * exp;
};

struct Or: Expression {
    Expression * lhs,
               * rhs;
};

struct And: Expression {
    Expression * lhs,
               * rhs;
};

struct Relational: Expression {
    Kind       kind;
    Expression * lhs,
               * rhs;
};

struct Arithmetic: Expression {
    Kind       kind;
    Expression * lhs,
               * rhs;
};

struct Unary: Expression {
    Kind       kind;
    Expression * sub;
};

struct Call: Expression {
    Expression                * sub;
    std::vector<Expression *> args;
};

struct GetElement: Expression {
    Expression * sub,
               * idx;
};

struct SetElement: Expression {
    Expression * sub,
               * idx,
               * val;
};

struct GetVariable: Expression {
    std::string name;
};

struct SetVariable: Expression {
    std::string name;
    Expression  * val;
};

struct NullLiteral: Expression {};

struct BooleanLiteral: Expression {
    bool val = false;
};

struct NumberLiteral: Expression {
    double val = 0.;
};

struct StringLiteral: Expression {
    std::string val;
};

struct ArrayLiteral: Expression {
    std::vector<Expression *> vals;
};

struct MapLiteral: Expression {
    std::map<std::string, Expression *> vals;
};