#include "Add.h"

#include <utility>
#include "AddVisitor.h"
#include <memory>

using GameSpecification::Add;

Add::Add(std::string to, std::unique_ptr<StateValue>& value) :
    BaseRule({}), 
    to(std::move(to)),
    value(std::move(value))
    {}

void Add::process(GameState& gameState){
    auto gameStateValue = gameState.getVariable(to);
    if (auto retrievedValue = gameStateValue->lock()) {
        AddVisitor visitor;
        retrievedValue->accept(visitor, value.get());
    }
}


