#ifndef SOCIALGAMING_STATEVALUE_H
#define SOCIALGAMING_STATEVALUE_H

#include <string>
#include <vector>
#include <unordered_map>

class GameStateVisitor;

class StateValue {
public:
    enum ValueType {
        STRING,
        NUMBER,
        BOOLEAN,
        LIST,
        MAP
    };
    virtual ValueType getValueType() const = 0;
    virtual std::string toString() const = 0;
    virtual void accept(GameStateVisitor& visitor) = 0;
    virtual void accept(GameStateVisitor& visitor, StateValue* stateValue) = 0;
};


#endif //SOCIALGAMING_STATEVALUE_H
