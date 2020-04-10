#include "StateValueMap.h"
#include "GameStateVisitor.h"

StateValueMap::StateValueMap() :
        valueType(StateValue::MAP)
{}

std::string StateValueMap::toString() const {
    return "<map>";
}

StateValueMap::StateValueMap(const std::unordered_map<std::string, std::shared_ptr<StateValue>>& valueMap)
    : valueType(StateValue::MAP), stateValueMap(valueMap)
    {}

StateValue::ValueType StateValueMap::getValueType() const {
    return valueType;
}

std::unordered_map<std::string, std::shared_ptr<StateValue>>& StateValueMap::getMap() {
    return stateValueMap;
}

std::optional<std::weak_ptr<StateValue>> StateValueMap::getValue(const std::string& key) {
    auto it = stateValueMap.find(key);
    return (it != stateValueMap.end()) ?
           std::make_optional(std::weak_ptr(it->second)) :
           std::nullopt;
}

void StateValueMap::accept(GameStateVisitor &visitor) {
    visitor.visit(*this);
}
void StateValueMap::accept(GameStateVisitor &visitor, StateValue* stateValue){

}

void StateValueMap::addValue(const std::string& key, StateValueBoolean& value) {
    stateValueMap.insert({key, std::make_shared<StateValueBoolean>(value)});
}

void StateValueMap::addValue(const std::string& key, StateValueNumber& value) {
    stateValueMap.insert({key, std::make_shared<StateValueNumber>(value)});
}

void StateValueMap::addValue(const std::string& key, StateValueString& value) {
    stateValueMap.insert({key, std::make_shared<StateValueString>(value)});
}

void StateValueMap::addValue(const std::string& key, StateValueMap& value) {
    stateValueMap.insert({key, std::make_shared<StateValueMap>(value)});
}

void StateValueMap::addValue(const std::string& key, StateValueList& value) {
    stateValueMap.insert({key, std::make_shared<StateValueList>(value)});
}
