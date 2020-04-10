#ifndef TIMER_H
#define TIMER_H

#include "BaseRule.h"
#include <optional>
namespace GameSpecification{
class Timer : public BaseRule{
    public:
        Timer(const std::list<std::shared_ptr<BaseRule>>&, double duration, const std::string& mode, bool flag);

       	void process(GameState&) override; 

    private:
        double duration;
        std::string mode;
        std::optional<bool> flag;
        //TODO: Add "rules" field when more details are provided
};
}
#endif
