#include "InputVote.h"

using GameSpecification::InputVote;
using json = nlohmann::json;

InputVote::InputVote(const std::string& user,const std::string& prompt, const std::string& choices, const std::string& result, const double timeout): 
   BaseRule({}), 
   user(user), 
   prompt(prompt), 
   choices(choices), 
   result(result), 
   timeout(timeout)
   {}

void InputVote::process(GameState& gameState){

}