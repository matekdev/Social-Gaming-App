#ifndef LOOP_H
#define LOOP_H

#include "BaseRule.h"

namespace GameSpecification{
    class Loop : public BaseRule{
        public:
            Loop(const std::list<std::shared_ptr<BaseRule>>&, const std::string& failCondition);

            void process(GameState&) override;
            
        private:
            std::string failCondition;
    };
}
#endif
