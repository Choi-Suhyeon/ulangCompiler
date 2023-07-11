#include "kind.hpp"

auto toKind(std::string str) -> Kind {
    return stringToKind.count(str) ? stringToKind.at(str) : Kind::Unknown;
}

auto toString(Kind kind) -> std::string {
    return kindToString.count(kind) ? kindToString.at(kind) : "";
}