#include "InputChoice.h"

#include <utility>


using GameSpecification::BaseRule;
using GameSpecification::InputChoice;
using json = nlohmann::json;

InputChoice::InputChoice(std::string to, std::string prompt, std::unique_ptr<StateValue>& choices, std::string result, double timeout) :
	BaseRule({}),
	to(std::move(to)),
	prompt(std::move(prompt)),
	choices(std::move(choices)),
	result(std::move(result)),
	timeout(timeout) 
	{}


void InputChoice::process(GameState& gameState){
	//TODO: add logic for this rule
}
