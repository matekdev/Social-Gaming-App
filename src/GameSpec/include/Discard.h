#ifndef DISCARD_H
#define DISCARD_H

#include "BaseRule.h"
namespace GameSpecification{
    class Discard : public BaseRule{
        public:

            Discard(std::string  from, const int count);

            void process(GameState&) override; 

        private:
            std::string from;
            int count;
    };
}
#endif
