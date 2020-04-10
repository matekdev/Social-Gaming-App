#ifndef INPARALLEL_H
#define INPARALLEL_H

#include "BaseRule.h"

namespace GameSpecification{
    class Inparallel : public BaseRule{
        public:
            Inparallel(const std::list<std::shared_ptr<BaseRule>>&);
            void process(GameState&) override;
        private:
        
    };
}

#endif
