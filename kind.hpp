#pragma once

#include <string>
#include <map>

enum class Kind {
    Unknown, EndOfToken,

    NullLiteral,
    TrueLiteral, FalseLiteral,
    NumberLiteral, StringLiteral,
    Identifier,

    Function, Return,
    Variable,
    For, Break, Continue,
    If, Elif, Else,
    Print, PrintLine,

    LogicalAnd, LogicalOr,
    Assignment,
    Add, Subtract,
    Multiply, Divide, Modulo,
    Equal, NotEqual,
    LessThan, GreaterThan,
    LessOrEqual, GreaterOrEqual,

    Comma, Colon, Semicolon,
    LeftParen, RightParen,
    LeftBrace, RightBrace, 
    LeftBraket, RightBraket,
};

static std::map<std::string, Kind> stringToKind = {
    {"#unknown", Kind::Unknown},
    {"#EndOfToken", Kind::EndOfToken},

    {"null", Kind::NullLiteral},
    {"true", Kind::TrueLiteral},
    {"false", Kind::FalseLiteral},
    {"#Number", Kind::NumberLiteral},
    {"#String", Kind::StringLiteral},
    {"#indentifier", Kind::Identifier},

    {"function", Kind::Function},
    {"return", Kind::Return},
    {"var", Kind::Variable},
    {"for", Kind::For},
    {"break", Kind::Break},
    {"continue", Kind::Continue},
    {"if", Kind::If},
    {"elif", Kind::Elif},
    {"else", Kind::Else},
    {"print", Kind::Print},
    {"printLine", Kind::PrintLine},

    {"and", Kind::LogicalAnd},
    {"or", Kind::LogicalOr},

    {"=", Kind::Assignment},

    {"+", Kind::Add},
    {"-", Kind::Subtract},
    {"*", Kind::Multiply},
    {"/", Kind::Divide},
    {"%", Kind::Modulo},

    {"==", Kind::Equal},
    {"!=", Kind::NotEqual},
    {"<", Kind::LessThan},
    {">", Kind::GreaterThan},
    {"<=", Kind::LessOrEqual},
    {">=", Kind::GreaterOrEqual},

    {",", Kind::Comma},
    {":", Kind::Colon},
    {";", Kind::Semicolon},
    {"(", Kind::LeftParen},
    {")", Kind::RightParen},
    {"{", Kind::LeftBrace},
    {"}", Kind::RightBrace},
    {"[", Kind::LeftBraket},
    {"]", Kind::RightBraket},
};