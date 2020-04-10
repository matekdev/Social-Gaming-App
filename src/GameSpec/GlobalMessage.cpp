#include "GlobalMessage.h"
#include "MessageParser.h"
#include "StateValueParser.h"
#include <iostream>

using GameSpecification::GlobalMessage;
using json = nlohmann::json;

GlobalMessage::GlobalMessage(const std::string& value) : 
    BaseRule({}), 
    value(value){}

void GlobalMessage::process(GameState& gameState) {
    auto messageParser = MessageParser(gameState, value);
    if (messageParser.hasVariable()){
        StateValueParser variable(gameState, messageParser.getVariableString().value());
        if (variable.isPerUserValue()){
            auto perUserValue = variable.getPerUserValue();
            if (perUserValue.has_value()){
                auto userValuePairList = perUserValue.value().get();
                std::string perUserString;
                for (const auto& userValuePair : userValuePairList){
                    perUserString.append(userValuePair.value->toString());
                    perUserString.append(" ");
                }
                gameState.addMessageToEntireSession(messageParser.replaceVariableString(perUserString));
            }
        } else {
            auto stateValueWeakPtr = variable.getStateValue();
            if (!stateValueWeakPtr.has_value()){
                return;
            }
            auto stateValuePtr = stateValueWeakPtr->lock();
            gameState.addMessageToEntireSession(messageParser.replaceVariableString(stateValuePtr->toString()));
        }
    } else {
        gameState.addMessageToEntireSession(messageParser.getMessage());
    }
}
