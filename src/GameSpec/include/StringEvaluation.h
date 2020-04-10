#ifndef STRINGEVALUATION_H
#define STRINGEVALUATION_H

#include <boost/algorithm/string.hpp>
#include "GameState.h"
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <unordered_map>
#include <memory>

enum Operator{
    EQUAL,
    DOT,
    CONTAINS,
    COLLECTS,
    NEGATION,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    UNKNOWN
};

#endif
