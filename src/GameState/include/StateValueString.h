
#ifndef SOCIALGAMING_STATEVALUESTRING_H
#define SOCIALGAMING_STATEVALUESTRING_H

#include <string>
#include "StateValue.h"

class StateValueString : public StateValue {
public:
    explicit StateValueString(std::string val);
    std::string& getValue();
    std::string getValueConst() const{
    	return value;
    }
    StateValue::ValueType getValueType() const override;

    void accept(GameStateVisitor &visitor) override;
    void accept(GameStateVisitor &visitor, StateValue* stateValue) override;

    std::string toString() const override;

private:
    std::string value;
    StateValue::ValueType valueType;
};


#endif //SOCIALGAMING_STATEVALUESTRING_H
