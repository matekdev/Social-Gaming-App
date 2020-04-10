//GameSpec.h
#ifndef GAMESPEC_H
#define GAMESPEC_H

#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <memory>

#include "Add.h"
#include "Deal.h"
#include "Discard.h"
#include "Extend.h"
#include "ForEach.h"
#include "GlobalMessage.h"
#include "Inparallel.h"
#include "InputChoice.h"
#include "InputText.h"
#include "InputVote.h"
#include "Loop.h"
#include "Message.h"
#include "Parallelfor.h"
#include "Reverse.h"
#include "Scores.h"
#include "Shuffle.h"
#include "Sort.h"
#include "Switch.h"
#include "Timer.h"
#include "When.h"

using GameSpecification::BaseRule;

namespace GameSpecification{
	class GameSpec{
		public:
		    GameSpec(const nlohmann::json&);
		    std::list<std::shared_ptr<BaseRule>> getRules() const;

		private:
		    std::list<std::shared_ptr<BaseRule>> rules; 

		    nlohmann::json readSpec(const nlohmann::json&); 
		    void processSpec(const nlohmann::json&);
			std::shared_ptr<BaseRule> recursivelyParseSpec(const nlohmann::json&);
		    void addRule(std::shared_ptr<BaseRule>);

            void createStateList(nlohmann::basic_json<> list, std::unique_ptr<StateValue> &ptr);

			//Rules with other Rules
			std::shared_ptr<BaseRule> createForEach(const nlohmann::json&, const std::list<std::shared_ptr<BaseRule>>&);
			std::shared_ptr<BaseRule> createInParallel(const nlohmann::json&, const std::list<std::shared_ptr<BaseRule>>&);
			std::shared_ptr<BaseRule> createLoop(const nlohmann::json&, const std::list<std::shared_ptr<BaseRule>>&);
			std::shared_ptr<BaseRule> createParallelFor(const nlohmann::json&, const std::list<std::shared_ptr<BaseRule>>&);
			std::shared_ptr<BaseRule> createTimer(const nlohmann::json&, const std::list<std::shared_ptr<BaseRule>>&);

			//Rules with multple cases
			std::shared_ptr<BaseRule> createSwitch(const nlohmann::json&, const std::vector<GameSpecification::SwitchCase>&);
			std::shared_ptr<BaseRule> createWhen(const nlohmann::json&, const std::vector<GameSpecification::WhenCase>&);

			//Rules which only adds functionality
			std::shared_ptr<BaseRule> createAdd(const nlohmann::json&);
			std::shared_ptr<BaseRule> createDeal(const nlohmann::json&);
			std::shared_ptr<BaseRule> createDiscard(const nlohmann::json&);
			std::shared_ptr<BaseRule> createExtend(const nlohmann::json&);
			std::shared_ptr<BaseRule> createGlobalMessage(const nlohmann::json&);
			std::shared_ptr<BaseRule> createInputChoice(const nlohmann::json&);
			std::shared_ptr<BaseRule> createInputText(const nlohmann::json&);
			std::shared_ptr<BaseRule> createInputVote(const nlohmann::json&);
			std::shared_ptr<BaseRule> createMessage(const nlohmann::json&);
			std::shared_ptr<BaseRule> createReverse(const nlohmann::json&);
			std::shared_ptr<BaseRule> createScores(const nlohmann::json&);
			std::shared_ptr<BaseRule> createShuffle(const nlohmann::json&);
			std::shared_ptr<BaseRule> createSort(const nlohmann::json&);
    };
}
#endif





