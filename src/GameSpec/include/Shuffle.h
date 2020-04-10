#ifndef SHUFFLE_H
#define SHUFFLE_H

#include "BaseRule.h"
#include <random>
#include <boost/range/algorithm.hpp>
#include <algorithm>
#include <boost/range/algorithm/random_shuffle.hpp>

namespace GameSpecification{
class Shuffle :public BaseRule{
    public:
        Shuffle(const std::string& list);

		void process(GameState&) override;
    private:
        std::string list;
        void parseRule(const nlohmann::json&);
};
}
#endif
