#include "Timer.h"

using GameSpecification::Timer;
using json = nlohmann::json;

Timer::Timer(const std::list<std::shared_ptr<BaseRule>>& childRules, double duration, const std::string& mode, bool flag) : 
    BaseRule(childRules), 
    duration(duration), 
    mode(mode), 
    flag(flag)
    {}

void Timer::process(GameState& gameState){
    //TODO: Add logic to process timer rule
}
