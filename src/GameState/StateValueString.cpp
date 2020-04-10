#include "StateValueString.h"
#include "GameStateVisitor.h"

StateValueString::StateValueString(std::string val) :
        value(std::move(val)),
        valueType(STRING)
{}

std::string& StateValueString::getValue() {
    return value;
}

StateValue::ValueType StateValueString::getValueType() const {
    return valueType;
}

void StateValueString::accept(GameStateVisitor &visitor) {
    visitor.visit(*this);
}
void StateValueString::accept(GameStateVisitor &visitor, StateValue* stateValue){

}

std::string StateValueString::toString() const {
    return value;
}
