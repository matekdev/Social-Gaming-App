#ifndef DEAL_H
#define DEAL_H

#include "BaseRule.h"
#include "GameState.h"

namespace GameSpecification{
    class Deal : public BaseRule{
        public:
            Deal(std::string  from, std::unique_ptr<StateValue>& to, int count);

            void process(GameState&) override;
            
        private:
            std::string from;
            std::unique_ptr<StateValue> to;
            int count;
            
    };
}

#endif
