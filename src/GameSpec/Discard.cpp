#include "Discard.h"

#include <utility>
#include "DiscardVisitor.h"

using GameSpecification::Discard;
using GameSpecification::BaseRule;

Discard::Discard(std::string  from, const int count) :
    BaseRule({}), 
    from(std::move(from)),
    count(count)
    {}

void Discard::process(GameState& gameState) {
    auto gameStateValueFrom = gameState.getVariable(from);
    if (auto fromList = gameStateValueFrom->lock()) {

        DiscardVisitor visitor(count);
        fromList->accept(visitor);
    }
}
