#ifndef SOCIALGAMING_MESSAGEPARSER_H
#define SOCIALGAMING_MESSAGEPARSER_H

#include "GameState.h"

class MessageParser {
public:
    MessageParser(GameState&, const std::string&);
    std::optional<std::string> getVariableString();
    std::string& getMessage();
    std::string replaceVariableString(const std::string& replacementValue);
    [[nodiscard]] bool hasVariable() const;
private:
    std::optional<std::string> findVariableString(const std::string& message);
    GameState& gameState;
    std::string message;
    std::optional<std::string> variableString;
};


#endif //SOCIALGAMING_MESSAGEPARSER_H
