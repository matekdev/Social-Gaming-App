#include "StateValueList.h"
#include "GameStateVisitor.h"

StateValueList::StateValueList() :
        valueType(StateValue::LIST)
{}


StateValueList::StateValueList(const std::vector<std::shared_ptr<StateValue>>& valueList)
    : valueType(StateValue::LIST), stateValueList(valueList)
    {}

StateValue::ValueType StateValueList::getValueType() const {
    return valueType;
}

std::vector<std::shared_ptr<StateValue>>& StateValueList::getList() {
    return stateValueList;
}

void StateValueList::accept(GameStateVisitor &visitor) {
    visitor.visit(*this);
}
void StateValueList::accept(GameStateVisitor &visitor, StateValue* stateValue){

}

void StateValueList::addValue(StateValueBoolean& value) {
    stateValueList.push_back(std::make_shared<StateValueBoolean>(value));
}

void StateValueList::addValue(StateValueNumber& value) {
    stateValueList.push_back(std::make_shared<StateValueNumber>(value));
}

void StateValueList::addValue(StateValueString& value) {
    stateValueList.push_back(std::make_shared<StateValueString>(value));
}

void StateValueList::addValue(StateValueMap& value) {
    stateValueList.push_back(std::make_shared<StateValueMap>(value));
}

void StateValueList::addValue(StateValueList& value) {
    stateValueList.push_back(std::make_shared<StateValueList>(value));
}

std::string StateValueList::toString() const {
    std::string str;
    for (auto& items : stateValueList){
        str.append(items->toString());
        str.append(" ");
    }
    return str;
}
