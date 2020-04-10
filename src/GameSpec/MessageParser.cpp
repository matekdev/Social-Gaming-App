#include "MessageParser.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>

MessageParser::MessageParser(GameState& gameState, const std::string& string) :
        gameState(gameState),
        message(string),
        variableString(findVariableString(message))
{}

std::optional<std::string> MessageParser::getVariableString() {
    return variableString;
}

std::optional<std::string> MessageParser::findVariableString(const std::string &message) {
    auto start = message.find('{');
    auto end = message.find('}');
    if (start == std::string::npos || end == std::string::npos || start >= end){
        return std::nullopt;
    }
    auto variable = message.substr(start+1, end-start-1);
    return std::make_optional(variable);
}

std::string MessageParser::replaceVariableString(const std::string &replacementValue) {
    if (!hasVariable()){
        return message;
    }
    std::string replacementString = message;
    std::string searchValue = '{' + variableString.value();
    searchValue.push_back('}');
    boost::replace_first(replacementString, searchValue, replacementValue);
    return replacementString;
}

bool MessageParser::hasVariable() const {
    return variableString.has_value();
}

std::string& MessageParser::getMessage() {
    return message;
}
