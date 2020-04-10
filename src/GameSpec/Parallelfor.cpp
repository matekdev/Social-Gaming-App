#include "Parallelfor.h"

using GameSpecification::Parallelfor;
using json = nlohmann::json;

Parallelfor::Parallelfor(const std::list<std::shared_ptr<BaseRule>>& childRules, const std::string& listName, const std::string& element) : 
    BaseRule(childRules), 
    listName(listName), 
    element(element)
    {}

void Parallelfor::process(GameState& gameState){
	//TODO: Add code to process Parallelfor
}
