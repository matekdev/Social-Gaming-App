#ifndef REVERSE_H
#define REVERSE_H

#include "BaseRule.h"
#include <boost/range/algorithm/reverse.hpp>
#include <iterator>
#include <algorithm>

namespace GameSpecification{

class Reverse : public BaseRule{
    public:

        Reverse(const std::string& list);

		void process(GameState&) override;
    private:
        std::string list;
};
}
#endif
