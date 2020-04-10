#include "Sort.h"
#include "SortVisitor.h"

using GameSpecification::Sort;
using json = nlohmann::json;

Sort::Sort(const std::string& list, const std::string& key) : 
    BaseRule({}), 
    list(list), 
    key(key)
    {}

void Sort::process(GameState& gameState){
    auto variables = gameState.getVariable(list);
    if (auto retrievedValue = variables->lock()) {
        // TODO: Finish sort visitor implementation
        SortVisitor visitor;
        retrievedValue->accept(visitor);
    }
}
