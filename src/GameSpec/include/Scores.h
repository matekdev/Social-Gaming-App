#ifndef SCORES_H
#define SCORES_H

#include "BaseRule.h"
namespace GameSpecification{
class Scores : public BaseRule{
    public:
        Scores(std::string  scoreValue, bool ascending);
		void process(GameState&) override;
    private:
        std::string score;
        bool ascending;
};
}
#endif
