#include "Loop.h"

using GameSpecification::Loop;
using json = nlohmann::json;

Loop::Loop(const std::list<std::shared_ptr<BaseRule>>&, const std::string& failCondition) : 
    BaseRule(childRules), 
    failCondition(failCondition)
    {}

void Loop::process(GameState& gameState){
	//TODO: add code to process loop rule
}
