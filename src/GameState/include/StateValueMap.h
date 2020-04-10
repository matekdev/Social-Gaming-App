#ifndef SOCIALGAMING_STATEVALUEMAP_H
#define SOCIALGAMING_STATEVALUEMAP_H

#include "StateValue.h"
#include "StateValueString.h"
#include "StateValueBoolean.h"
#include "StateValueNumber.h"
#include "StateValueList.h"
#include <unordered_map>
#include <memory>
#include <optional>

class StateValueMap : public StateValue {
public:
    StateValueMap();
    explicit StateValueMap(const std::unordered_map<std::string, std::shared_ptr<StateValue>>&);
    StateValue::ValueType getValueType() const override;
    std::unordered_map<std::string, std::shared_ptr<StateValue>>& getMap();
    std::optional<std::weak_ptr<StateValue>> getValue(const std::string&);
    std::optional<std::weak_ptr<StateValue>> getValueConst(const std::string& key) const{
        auto it = stateValueMap.find(key);
        return (it != stateValueMap.end()) ?
               std::make_optional(std::weak_ptr(it->second)) :
               std::nullopt;
    }
    void addValue(const std::string&, StateValueBoolean&);
    void addValue(const std::string&, StateValueNumber&);
    void addValue(const std::string&, StateValueString&);
    void addValue(const std::string&, StateValueMap&);
    void addValue(const std::string&, StateValueList&);

    void accept(GameStateVisitor &visitor) override;
    void accept(GameStateVisitor &visitor, StateValue* stateValue) override;

    std::string toString() const override;

private:
    StateValue::ValueType valueType;
    std::unordered_map<std::string, std::shared_ptr<StateValue>> stateValueMap;
};

#endif //SOCIALGAMING_STATEVALUEMAP_H
