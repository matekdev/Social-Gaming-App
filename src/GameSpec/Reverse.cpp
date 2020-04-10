#include "Reverse.h"
#include "ReverseVisitor.h"

using GameSpecification::Reverse;
using GameSpecification::BaseRule;
using json = nlohmann::json;


Reverse::Reverse(const std::string& list) : 
    BaseRule({}), 
    list(list) 
    {}
    
void Reverse::process(GameState& gameState) {
    auto variables = gameState.getVariable(list);
    if(auto retrievedValue = variables->lock()){
        ReverseVisitor visitor;
        retrievedValue->accept(visitor);
    }
}
