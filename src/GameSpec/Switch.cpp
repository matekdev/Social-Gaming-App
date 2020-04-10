#include "Switch.h"
using GameSpecification::Switch;
using GameSpecification::BaseRule;
using GameSpecification::SwitchCase;
using json = nlohmann::json;

Switch::Switch(const std::string& value, const std::string& listName, const std::vector<SwitchCase>& cases) : 
    BaseRule({}), 
    value(value),
    listName(listName)
    {}

void Switch::process(GameState&){

}

SwitchCase::SwitchCase(const std::string& value, const std::list<std::shared_ptr<BaseRule>>& rules) :
    value(value),
    rules(rules)
    {}
