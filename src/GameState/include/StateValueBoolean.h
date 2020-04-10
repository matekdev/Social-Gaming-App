#ifndef SOCIALGAMING_STATEVALUEBOOLEAN_H
#define SOCIALGAMING_STATEVALUEBOOLEAN_H

#include "StateValue.h"

class StateValueBoolean : public StateValue {
public:
    explicit StateValueBoolean(bool val);
    bool& getValue();
    bool getValueConst() const{
    	return value;
    }
    StateValue::ValueType getValueType() const override;
    void accept(GameStateVisitor &visitor) override;
    void accept(GameStateVisitor &visitor, StateValue* stateValue) override;

    std::string toString() const override;

private:
    bool value;
    StateValue::ValueType valueType;
};

#endif //SOCIALGAMING_STATEVALUEBOOLEAN_H
