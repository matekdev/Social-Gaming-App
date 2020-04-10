#include "Deal.h"

#include <utility>
#include "DealVisitor.h"

using GameSpecification::Deal;
using GameSpecification::BaseRule;

Deal::Deal(std::string  from, std::unique_ptr<StateValue>& to, const int count) :
    BaseRule({}), 
    from(std::move(from)),
    to(std::move(to)),
    count(count) 
    {}

void Deal::process(GameState& gameState) {
	//TODO: Add code to process deal rule
    auto gameStateValueFrom = gameState.getVariable(from);
    
//    if (auto fromList = gameStateValueFrom->lock()) {
//        auto gameStateValueTo = gameState.getVariable(boost::get<std::string>(to.value));
//        if (auto toList = gameStateValueTo->lock()) {
//            std::shared_ptr<StateValueList> valueList;
//            valueList = std::static_pointer_cast<StateValueList>(toList);
//
//            DealVisitor visitor(count, *valueList);
//            fromList->accept(visitor);
//        }
//    }
}