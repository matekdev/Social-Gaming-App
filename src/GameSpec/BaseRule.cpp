#include "BaseRule.h"

using GameSpecification::BaseRule;
using nlohmann::json;

BaseRule::BaseRule(const std::list<std::shared_ptr<BaseRule>>& childs)
 : childRules(childs) {}


std::list<std::shared_ptr<BaseRule>> BaseRule::getNextPtr() const noexcept {
	return next;	
}


void BaseRule::setNextPtr(const std::list<std::shared_ptr<BaseRule>>& ptrList){
	next = ptrList;
}

void BaseRule::addNextPtr(const std::shared_ptr<BaseRule>& ptr){
	next.push_back(ptr);
}
