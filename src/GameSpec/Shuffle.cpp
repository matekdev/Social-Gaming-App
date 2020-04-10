#include "Shuffle.h"
#include "ShuffleVisitor.h"
using GameSpecification::Shuffle;
using GameSpecification::BaseRule;
using json = nlohmann::json;

Shuffle::Shuffle(const std::string& list) : 
    BaseRule({}), 
    list(list)
    {}
    
void Shuffle::process(GameState& gameState) {
    auto variables = gameState.getVariable(list);
    if (auto retrievedValue = variables->lock()) {
        ShuffleVisitor visitor;
        retrievedValue->accept(visitor);
    }
}
