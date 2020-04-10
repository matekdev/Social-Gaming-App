#include "StateValueNumber.h"
#include "GameStateVisitor.h"

StateValueNumber::StateValueNumber(int val) :
        value(val),
        valueType(NUMBER)
{}

StateValueNumber::StateValueNumber(double val) :
        value(val),
        valueType(NUMBER)
{}

int& StateValueNumber::getValue() {
    return value;
}

StateValue::ValueType StateValueNumber::getValueType() const {
    return valueType;
}

void StateValueNumber::accept(GameStateVisitor &visitor) {
    visitor.visit(*this);
}
void StateValueNumber::accept(GameStateVisitor &visitor, StateValue* stateValue){
    StateValueNumber* t = static_cast<StateValueNumber*>(stateValue);
    visitor.visit(*this, *t);
}

std::string StateValueNumber::toString() const {
    return std::to_string(value);
}
