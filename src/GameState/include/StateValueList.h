#ifndef SOCIALGAMING_STATEVALUELIST_H
#define SOCIALGAMING_STATEVALUELIST_H

#include <memory>
#include "StateValue.h"
#include "StateValueString.h"
#include "StateValueBoolean.h"
#include "StateValueNumber.h"
class StateValueMap;

class StateValueList : public StateValue {
public:
    StateValueList();

    explicit StateValueList(const std::vector<std::shared_ptr<StateValue>>&);

    StateValue::ValueType getValueType() const override;

    std::vector<std::shared_ptr<StateValue>>& getList();
    std::vector<std::shared_ptr<StateValue>> getListConst() const{
        return stateValueList;
    }
    void addValue(StateValueBoolean&);
    void addValue(StateValueNumber&);
    void addValue(StateValueString&);
    void addValue(StateValueMap&);
    void addValue(StateValueList&);

    void accept(GameStateVisitor &visitor) override;
    void accept(GameStateVisitor &visitor, StateValue* stateValue) override;

    std::string toString() const override;

private:
    StateValue::ValueType valueType;
    std::vector<std::shared_ptr<StateValue>> stateValueList;
};

#endif //SOCIALGAMING_STATEVALUELIST_H
