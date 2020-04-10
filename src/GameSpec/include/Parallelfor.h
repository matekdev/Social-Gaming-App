#ifndef PARALLELFOR_H
#define PARALLELFOR_H

#include "BaseRule.h"

namespace GameSpecification{
    class Parallelfor : public BaseRule{
        public:
            Parallelfor(const std::list<std::shared_ptr<BaseRule>>&, const std::string&, const std::string&);

            void process(GameState&) override;        
        private:
            std::string listName;
            std::string element;
    };
}
#endif
