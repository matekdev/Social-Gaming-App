#include "Inparallel.h"

using GameSpecification::Inparallel;

Inparallel::Inparallel(const std::list<std::shared_ptr<BaseRule>>& childRules) :
	BaseRule(childRules)
	{}

void Inparallel::process(GameState& gameState){
	//TODO: Add code to process inparallel
}
