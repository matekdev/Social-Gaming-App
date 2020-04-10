#ifndef ADD_H
#define ADD_H

#include "BaseRule.h"
#include "GameState.h"
#include <memory>

namespace GameSpecification{
    class Add : public BaseRule{
        public:
            Add(std::string to, std::unique_ptr<StateValue>& value);

            void process(GameState&) override;

        private:
            std::string to;
            std::unique_ptr<StateValue> value;

//            int determineAmountToAdd(const StateValue &, GameState &);
    };
}

#endif
