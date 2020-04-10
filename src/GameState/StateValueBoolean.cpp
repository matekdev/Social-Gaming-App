#include "StateValueBoolean.h"
#include "GameStateVisitor.h"

StateValueBoolean::StateValueBoolean(bool val) :
        value(val),
        valueType(BOOLEAN)
{}

bool& StateValueBoolean::getValue() {
    return value;
}

StateValue::ValueType StateValueBoolean::getValueType() const {
    return valueType;
}

void StateValueBoolean::accept(GameStateVisitor &visitor) {
    visitor.visit(*this);
}

void StateValueBoolean::accept(GameStateVisitor &visitor, StateValue* stateValue){

}

std::string StateValueBoolean::toString() const {
    return value ? "true" : "false";
}
