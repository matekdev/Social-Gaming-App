#ifndef GAMESPECHELPER_H
#define GAMESPECHELPER_H
#include <vector>
#include <algorithm>
#include "BaseRule.h"

using nlohmann::json;

namespace GameSpecification{
//Note: This function are not dealing with ForEach, ParallelFor, Loop, Inparallelfor
//		because when calling this function inside those classes, it would cause "invalid use of incomplete type"
static std::shared_ptr<BaseRule> getRulePtrFromRuleType(RuleType ruleType, const json &ruleJson){
	switch(ruleType){
		case RuleType::AddType: 
			return std::shared_ptr<BaseRule>(new Add(ruleJson));
		case RuleType::ExtendType:
			return std::shared_ptr<BaseRule>(new Extend(ruleJson));
		case RuleType::ReverseType:
			return std::shared_ptr<BaseRule>(new Reverse(ruleJson));
		case RuleType::ShuffleType:
			return std::shared_ptr<BaseRule>(new Shuffle(ruleJson));
		case RuleType::SortType:
			return std::shared_ptr<BaseRule>(new Sort(ruleJson));
		case RuleType::DealType:
			return std::shared_ptr<BaseRule>(new Deal(ruleJson));
		case RuleType::DiscardType:
			return std::shared_ptr<BaseRule>(new Discard(ruleJson));
		case RuleType::InputChoiceType:
			return std::shared_ptr<BaseRule>(new InputChoice(ruleJson));
		case RuleType::InputTextType:
			return std::shared_ptr<BaseRule>(new InputText(ruleJson));
		case RuleType::InputVoteType:
			return std::shared_ptr<BaseRule>(new InputVote(ruleJson));
		case RuleType::MessageType:
			return std::shared_ptr<BaseRule>(new Message(ruleJson));
		case RuleType::ScoresType:
			return std::shared_ptr<BaseRule>(new Scores(ruleJson));
		case RuleType::TimerType:
			return std::shared_ptr<BaseRule>(new Timer(ruleJson));
		case RuleType::GlobalMessageType:
			return std::shared_ptr<BaseRule>(new GlobalMessage(ruleJson));
		default:
			return nullptr;
	}
	*/

}


#endif
