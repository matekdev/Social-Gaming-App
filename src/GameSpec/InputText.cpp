#include "InputText.h"

using GameSpecification::InputText;
using json = nlohmann::json;

InputText::InputText(const std::string& user, const std::string& prompt, const std::string& result, const double timeout) : 
    BaseRule({}), 
    user(user), 
    prompt(prompt), 
    result(result), 
    timeout(timeout)
    {}

void InputText::process(GameState& gameState){
       
}
