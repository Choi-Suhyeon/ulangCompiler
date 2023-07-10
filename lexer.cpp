#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "token.hpp"
#include "kind.hpp"

enum class CharType {
    Unknown,
    WhiteSpace,
    NumberLiteral,
    StringLiteral,
    IdentifierAndKeyword,
    OperatorAndPunctuator,
};

auto analyzeLexeme(std::string source_code) -> std::vector<Token> {
    using std::regex_search;
    using std::vector;
    using std::smatch;
    using std::regex;
    using std::string;

    auto result = vector<Token>();

    source_code += '\0';

    auto re_white_space = regex(R"(^[ \r\t\n]+)");
    auto re_num_literal = regex(R"(^\d+(\.\d+)?)");
    auto re_str_literal = regex(R"(^'.*')");
    auto re_id_and_kywd = regex(R"(^[[:alpha:]][[:alnum:]]*)");
    auto re_op_and_punc = regex(R"(^\+|\-|\*|\/|;|\(|\)|\{|\})");
    smatch match;

    while (source_code[0]) {
        if (regex_search(source_code, match, re_white_space)) {
        }
        else if (regex_search(source_code, match, re_num_literal)) {
            result.push_back({Kind::NumberLiteral, match.str()});
        }
        else if (regex_search(source_code, match, re_str_literal)) {
            result.push_back({Kind::StringLiteral, match.str()});
        }
        else if (regex_search(source_code, match, re_id_and_kywd)) {
            auto matched  = match.str();
            auto str_kind = stringToKind.find(matched);

            result.push_back({
                str_kind == stringToKind.end() ? Kind::Identifier : str_kind->second,
                matched,
            });
        }
        else if (regex_search(source_code, match, re_op_and_punc)) {
            result.push_back({stringToKind.at(match.str()), match.str()});
        }
        else {
            std::cout << "unkown substring.\n";
            exit(1);
        }

        source_code = match.suffix();
    }

    result.push_back({Kind::EndOfToken});

    return result;
}

auto main() -> int {
    std::string sc = R""""(
        function main() {
            printLine 'Hello, World!';
            printLine 1 * 2 + 3;
        }
    )"""";

    auto rslt = analyzeLexeme(sc);
    for (auto tokn : rslt) {
        std::string kind_string = "";

        for (auto & i : stringToKind) {
            if (i.second == tokn.kind) {
                kind_string = i.first;
            }
        }
        std::cout << kind_string << " :: " << tokn.str << std::endl;
    }
}