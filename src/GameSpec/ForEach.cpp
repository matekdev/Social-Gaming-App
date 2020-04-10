#include "ForEach.h"

#include <utility>

using GameSpecification::ForEach;
//using GameSpecification::SpecValue;
//using GameSpecification::BaseRule;
using json = nlohmann::json;

ForEach::ForEach(const std::list<std::shared_ptr<BaseRule>>& childRules, std::unique_ptr<StateValue>& list, std::string element) :
	BaseRule(childRules), 
	list(std::move(list)),
	element(std::move(element))
	{}

//TODO: Add code to process foreach rule
void ForEach::process(GameState& gameState){

}
