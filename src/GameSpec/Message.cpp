#include "Message.h"

#include <utility>
#include "MessageVisitor.h"

using GameSpecification::Message;
using GameSpecification::BaseRule;

Message::Message(std::unique_ptr<StateValue>& to, std::string val) :
    BaseRule({}),
    to(std::move(to)),
    messValue(std::move(val))
    {}

void Message::process(GameState& gameState){
    auto variables = gameState.getVariable(messValue);
    if (auto retrievedValue = variables->lock()) {
        MessageVisitor visitor;
        retrievedValue->accept(visitor);
	}
}
