#include "When.h"
#include "StringEvaluation.cpp"

#include <utility>
using GameSpecification::When;
using GameSpecification::BaseRule;
using GameSpecification::WhenCase;

When::When(std::vector<WhenCase>  cases) :
        BaseRule({}),
        cases(std::move(cases))
{}

void When::process(GameState& gameState){
    for (const auto& whenCase : cases) {
        bool evaluatedValue = evaluate(gameState, whenCase.condition);
        if (evaluatedValue) {
            for (const auto& rule : whenCase.rules) {
                rule->process(gameState);
            }
        }
    }
}

WhenCase::WhenCase(std::string condition, std::list<std::shared_ptr<BaseRule>>  rules) :
        condition(std::move(condition)),
        rules(std::move(rules))
{}
