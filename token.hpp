#pragma once

#include <string>
#include "kind.hpp"

struct Token {
    Kind kind = Kind::Unknown;
    std::string str;
};