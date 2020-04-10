#include "StateValueParser.h"
#include <boost/algorithm/string.hpp>
#include <iostream>

StateValueParser::StateValueParser(GameState& gameState, const std::string& string) :
    gameState(gameState),
    string(string)
{
     boost::algorithm::split(splitString, string, boost::is_any_of("."));
}

std::optional<std::weak_ptr<const StateValue>> StateValueParser::getStateValue() {
    if (isPerUserValue()){
        return std::nullopt;
    }
    auto currVariable = gameState.getConstantOrVariable(splitString[0]);
    for (auto i = splitString.begin()+1; i < splitString.end(); i++){
        if (!currVariable.has_value()){
            return std::nullopt;
        }
        if (auto v = currVariable.value().lock()){
            if (v->getValueType() == StateValue::MAP){
                auto variable = static_cast<const StateValueMap*>(v.get());
                currVariable = variable->getValueConst(*i);
            } else {
                return std::nullopt;
            }
        } else {
            return std::nullopt;
        }
    }
    return currVariable;
}

bool StateValueParser::isPerUserValue(){
    if (!splitString.empty()){
        return splitString[0] == PLAYERS || splitString[0] == AUDIENCE;
    }
    return false;
}


std::optional<std::reference_wrapper<std::vector<GameState::StateValueUserPair>>> StateValueParser::getPerUserValue() {
    if (!isPerUserValue() || splitString.size() < 2){
        return std::nullopt;
    }
    auto currVariable = gameState.getPerUserValue(splitString[1]);
    for (auto i = splitString.begin()+2; i < splitString.end(); ++i){
        if (!currVariable.has_value()){
            return std::nullopt;
        }
        auto v = currVariable.value().get();
        std::transform(v.begin(), v.end(), v.begin(), [&i](const GameState::StateValueUserPair& pair){
            auto stateValueMap = static_cast<const StateValueMap*>(pair.value.get());
            auto nextValue = stateValueMap->getValueConst(*i);
            if (nextValue.has_value()){
                return pair;
            }
            GameState::StateValueUserPair newPair{pair.user, nextValue->lock()};
            return newPair;
        });
    }
    return currVariable;
}
