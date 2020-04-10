#include "GameSpec.h"
#include <iterator>
#include <algorithm>
#include <fstream>

using GameSpecification::GameSpec;
using json = nlohmann::json;

namespace SpecTags{
    std::string ASCENDING = "ascending";
    std::string CASES = "cases";
    std::string CHOICES = "choices";
    std::string CONDITION = "condition";
    std::string COUNT = "count";
    std::string DURATION = "duration";
    std::string ELEMENT = "element";
    std::string FROM = "from";
    std::string KEY = "key";
    std::string LIST = "list";
    std::string MODE = "mode";
    std::string PROMPT = "prompt";
    std::string RESULT = "result";
    std::string RULE_LIST = "rules";
    std::string RULE_NAME = "rule";
    std::string SCORE = "score";
    std::string TARGET = "target";
    std::string TIMEOUT = "timeout";
    std::string TO = "to";
    std::string UNTIL = "until";


    std::string VALUE = "value";
    std::string WHILE = "while";

    std::string TRACK = "track"; //not a tag?
}

namespace RuleTags{
	const std::string Add = "add";
    const std::string Deal = "deal";
    const std::string Discard = "discard";
    const std::string Extend = "extend";
    const std::string ForEach = "foreach"; //contains child rules
    const std::string GlobalMessage = "global-message";
    const std::string Inparallel = "inparallel"; //contains child rules
    const std::string InputChoice = "input-choice";
    const std::string InputText = "input-text";
    const std::string InputVote = "input-vote";
    const std::string Loop = "loop"; //contains child rules
    const std::string Message = "message"; 
    const std::string Parallelfor = "parallelfor"; //contains child rules
    const std::string Reverse = "reverse";

    const std::string Scores = "scores";
    const std::string Shuffle = "shuffle";
    const std::string Sort = "sort";
    const std::string Switch = "switch"; //contains case rules 
    const std::string Timer = "timer"; //contains child rules
    const std::string When = "when"; //contains case rules 
}

GameSpec::GameSpec(const nlohmann::json& fullFileJson) : rules({}){
	nlohmann::json rulesJson = readSpec(fullFileJson);
	processSpec(rulesJson);
}


std::list<std::shared_ptr<BaseRule>> GameSpec::getRules() const{
	return rules;
}

void GameSpec::addRule(std::shared_ptr<BaseRule> rule){
	rules.push_back(rule);
}

nlohmann::json GameSpec::readSpec(const nlohmann::json& fullFileJson){
	return fullFileJson.at(SpecTags::RULE_LIST);
}

void GameSpec::processSpec(const nlohmann::json& ruleJson){
	std::vector<nlohmann::json> ruleList = ruleJson.get<std::vector<nlohmann::json>>();

	for(auto& rule : ruleList){
		auto parsedRule = recursivelyParseSpec(rule);
		addRule(parsedRule);
	}
}

std::shared_ptr<BaseRule> GameSpec::recursivelyParseSpec(const nlohmann::json& currentRuleJson){
	std::string ruleType = currentRuleJson
		.at(SpecTags::RULE_NAME)
		.get<std::string>();	

	std::shared_ptr<BaseRule> result;

	if(currentRuleJson.contains(SpecTags::RULE_LIST)){
		//these rules have child rules so we need to do more complex processing
		//this part is the recursive part

		std::list<std::shared_ptr<BaseRule>> childRules {};
		
		//every rule list has an array of rules
		auto subRules = currentRuleJson
			.at(SpecTags::RULE_LIST);
		
		//recursively parse every rule in the array of rules
		for (auto& subRule : subRules){
			auto singleChildrule = recursivelyParseSpec(subRule);
			childRules.push_back(singleChildrule);
		} 

		//configure make each (n)th childRule point to (n+1)th childRule, ignore last one
		for(auto it = childRules.begin(); it != childRules.end(); ++it){
			if(it == childRules.end()) break;
		    (*it)->addNextPtr(*(std::next(it, 1)));
		}

		//get params and setup rule with the child list, assign to result
		if(ruleType == RuleTags::ForEach){
            result = createForEach(currentRuleJson, childRules);
		}
		else if(ruleType == RuleTags::Inparallel){
            result = createInParallel(currentRuleJson, childRules);
		}
        else if(ruleType == RuleTags::Loop){
            result = createLoop(currentRuleJson, childRules);
        }
		else if(ruleType == RuleTags::Parallelfor){
            result = createParallelFor(currentRuleJson, childRules);
        }
        else if(ruleType == RuleTags::Timer){
            result = createTimer(currentRuleJson, childRules);
        } 
		else{
			//something horrible happened
			assert(false);
		}

		//make last child point back to us
		auto lastChild = childRules.rbegin();
		if(lastChild != childRules.rend()){
			(*lastChild)->addNextPtr(result);
		}		
	}
    else if(currentRuleJson.contains(SpecTags::CASES)){
        std::vector<nlohmann::json> caseList = currentRuleJson
            .at(SpecTags::CASES)
            .get<std::vector<nlohmann::json>>();
        
        if(ruleType == RuleTags::Switch){
            std::vector<GameSpecification::SwitchCase> switchCases {};

            for(auto& singleCase : caseList){
                std::string value = singleCase
                    .at(SpecTags::VALUE)
                    .get<std::string>();
                
                std::vector<nlohmann::json> subRules = singleCase
                    .at(SpecTags::RULE_LIST)
                    .get<std::vector<nlohmann::json>>();

           		std::list<std::shared_ptr<BaseRule>> childRules {};

                for (auto& subRule : subRules){
                    auto singleChildrule = recursivelyParseSpec(subRule);
                    childRules.push_back(singleChildrule);
                }                   
                
                switchCases.push_back(GameSpecification::SwitchCase(value, childRules));
            }

            result = createSwitch(currentRuleJson, switchCases);
        } 
        else if(ruleType == RuleTags::When){
            std::vector<GameSpecification::WhenCase> whenCases {};

            for(auto& singleCase : caseList){
                std::string condition = singleCase
                    .at(SpecTags::CONDITION)
                    .get<std::string>();
                
                std::vector<nlohmann::json> subRules = singleCase
                    .at(SpecTags::RULE_LIST)
                    .get<std::vector<nlohmann::json>>();

           		std::list<std::shared_ptr<BaseRule>> childRules {};

                for (auto& subRule : subRules){
                    auto singleChildrule = recursivelyParseSpec(subRule);
                    childRules.push_back(singleChildrule);
                }                   
                
                whenCases.push_back(GameSpecification::WhenCase(condition, childRules));
            }

            result = createWhen(currentRuleJson, whenCases);
        }
		else{
			//something horrible happened
			assert(false);
		}
    }
	else{
		//these rules should not have childs, so their processing is quite simple
		//these are the non-recursive parts
        //1 rules field. only .

		if(ruleType == RuleTags::Add){

            result = createAdd(currentRuleJson);
		}		
        else if(ruleType == RuleTags::Deal) {
            result = createDeal(currentRuleJson);
        }

		else if(ruleType == RuleTags::Discard){
            result = createDiscard(currentRuleJson);
        }
        else if(ruleType == RuleTags::Extend){
            result = createExtend(currentRuleJson);
        } 
        else if(ruleType == RuleTags:: GlobalMessage){
            result = createGlobalMessage(currentRuleJson);
        } 
        else if(ruleType == RuleTags::InputChoice){
            result = createInputChoice(currentRuleJson);
        } 
        else if(ruleType == RuleTags::InputText){
            result = createInputText(currentRuleJson);
        } 
        else if(ruleType == RuleTags::InputVote){
            result = createInputVote(currentRuleJson);
        } 
        else if(ruleType == RuleTags::Message){
            result = createMessage(currentRuleJson);
        } 
        else if(ruleType == RuleTags::Reverse){
            result = createReverse(currentRuleJson);
        }

        else if(ruleType == RuleTags::Scores){
            result = createScores(currentRuleJson);
        } 
        else if(ruleType == RuleTags::Shuffle){
            result = createShuffle(currentRuleJson);
        } 
        else if(ruleType == RuleTags::Sort){
            result = createSort(currentRuleJson);
        } 

    	else{
			//something horrible happened
			assert(false);
		}
	}

	return result;
}

void GameSpec::createStateList(const json list,std::unique_ptr<StateValue> &ptr) {//TODO: move this inside StateList.
    std::vector<std::shared_ptr<StateValue>> listValue;
    std::transform(list.begin(), list.end(),
       std::back_inserter(listValue),
       [](const json& listElem){
                       if(listElem.is_number()){
                           int value = listElem.get<int>();
                           return std::shared_ptr<StateValue>(new StateValueNumber(value));
                       }else if(listElem.is_string()){
                           std::string value = listElem.get<std::string>();
                           return std::shared_ptr<StateValue>(new StateValueString(value));
                       }else{
                           std::cout << "Unhandled list element type\n";
                           assert(false);
                       }
                   });

    ptr = std::make_unique<StateValueList>(listValue);
}

//-------------------------------
//Rules which contain other rules
//-------------------------------
std::shared_ptr<BaseRule> GameSpec::createForEach(const nlohmann::json& currentRuleJson, const std::list<std::shared_ptr<BaseRule>>& childRules){
    auto element = currentRuleJson.at(SpecTags::ELEMENT).get<std::string>();
    auto listJson = currentRuleJson.at(SpecTags::LIST);
    std::unique_ptr<StateValue> listPtr;
    
    if(listJson.is_string()){
        listPtr = std::unique_ptr<StateValue>(new StateValueString(listJson.get<std::string>()));
    }
    else if(listJson.is_array()){
        createStateList(listJson,listPtr);
    }
    else{
        assert(false && "For-Each must be a string or array");
    }
    
    return std::shared_ptr<BaseRule>(new ForEach(childRules, listPtr, element));
}

std::shared_ptr<BaseRule> GameSpec::createInParallel(const nlohmann::json& currentRuleJson, const std::list<std::shared_ptr<BaseRule>>& childRules){
    return std::shared_ptr<BaseRule>(new Inparallel(childRules));
}

std::shared_ptr<BaseRule> GameSpec::createLoop(const nlohmann::json& currentRuleJson, const std::list<std::shared_ptr<BaseRule>>& childRules){
    std::string condition;

    if(currentRuleJson.contains(SpecTags::WHILE)){
        condition = currentRuleJson
            .at(SpecTags::WHILE)
            .get<std::string>();
    }
    else if(currentRuleJson.contains(SpecTags::UNTIL)){
        condition = currentRuleJson
            .at(SpecTags::UNTIL)
            .get<std::string>();
    }
    else{
        assert(false && "No condition detected in Loop");
    }
    
    return std::shared_ptr<BaseRule>(new Loop(childRules, condition));
}

std::shared_ptr<BaseRule> GameSpec::createParallelFor(const nlohmann::json& currentRuleJson, const std::list<std::shared_ptr<BaseRule>>& childRules){
    std::string listName = currentRuleJson
        .at(SpecTags::LIST)
        .get<std::string>();

    std::string elementName = currentRuleJson
        .at(SpecTags::ELEMENT)
        .get<std::string>();
    
    return std::shared_ptr<BaseRule>(new Parallelfor(childRules, listName, elementName));
}

std::shared_ptr<BaseRule> GameSpec::createTimer(const nlohmann::json& currentRuleJson, const std::list<std::shared_ptr<BaseRule>>& childRules){

    double duration = currentRuleJson
        .at(SpecTags::DURATION)
        .get<std::double_t>();
    
    std::string mode = currentRuleJson
        .at(SpecTags::MODE)
        .get<std::string>();

    bool flag = (mode == SpecTags::TRACK);
    return std::shared_ptr<BaseRule>(new Timer(childRules, duration, mode,flag));
}

//------------------------
//Rules with multple cases
//------------------------

std::shared_ptr<BaseRule> GameSpec::createSwitch(const nlohmann::json& currentRuleJson, const std::vector<GameSpecification::SwitchCase>& cases){
    std::string value = currentRuleJson
        .at(SpecTags::VALUE)
        .get<std::string>();
    
    std::string listName = currentRuleJson
        .at(SpecTags::LIST)
        .get<std::string>();

    return std::shared_ptr<BaseRule>(new Switch(value, listName, cases));
}

std::shared_ptr<BaseRule> GameSpec::createWhen(const nlohmann::json& currentRuleJson, const std::vector<GameSpecification::WhenCase>& cases){   
    return std::shared_ptr<BaseRule>(new When(cases));
}

//-----------------------------------
//Rules which only adds functionality
//-----------------------------------
std::shared_ptr<BaseRule> GameSpec::createAdd(const nlohmann::json& currentRuleJson){
    std::string to = currentRuleJson.at(SpecTags::TO);

    // Determine if value is number literal.
    if (currentRuleJson.at(SpecTags::VALUE).is_number()) {
        double value = currentRuleJson.at(SpecTags::VALUE);
        std::unique_ptr<StateValue> ptr = std::make_unique<StateValueNumber>(value);
        return std::make_shared<GameSpecification::Add>(GameSpecification::Add(to, ptr));
    }

    std::string value = currentRuleJson.at(SpecTags::VALUE);
    std::unique_ptr<StateValue> ptr = std::make_unique<StateValueString>(value);
    return std::make_shared<GameSpecification::Add>(GameSpecification::Add(to, ptr));
}

std::shared_ptr<BaseRule> GameSpec::createDeal(const nlohmann::json& currentRuleJson){
    std::unique_ptr<StateValue> stateValuePointer = nullptr;
    std::vector<std::shared_ptr<StateValue>> listInRule;
    std::string from = currentRuleJson.at( SpecTags::FROM);
    auto to = currentRuleJson.at(SpecTags::TO);
    createStateList(to,stateValuePointer);//changes statevaluePointer to stateList with populated List
    int count = currentRuleJson.at(SpecTags::COUNT);    
    return std::make_shared<GameSpecification::Deal>(GameSpecification::Deal(from, stateValuePointer,count));;
}

std::shared_ptr<BaseRule> GameSpec::createDiscard(const nlohmann::json& currentRuleJson){
    std::string from = currentRuleJson.at(SpecTags::FROM).get<std::string>();
    int count = currentRuleJson.at(SpecTags::COUNT).get<int>();
    return std::shared_ptr<BaseRule>(new Discard(from, count));
}

std::shared_ptr<BaseRule> GameSpec::createExtend(const nlohmann::json& currentRuleJson){
    std::unique_ptr<StateValue> stateValuePointer = nullptr;
    std::string target = currentRuleJson.at( SpecTags::TARGET);
    auto list = currentRuleJson.at( SpecTags::LIST);

    if(list.is_array()) {
        std::vector<std::shared_ptr<StateValue>> listValue;
        createStateList(list,stateValuePointer);//ptr is reference
    } else if(list.is_string()) {
        stateValuePointer = std::make_unique<StateValueString>(list);
    }

    if(stateValuePointer) {
        return std::make_shared<GameSpecification::Extend>(GameSpecification::Extend(target, stateValuePointer));
    }
    
    return nullptr;
}

std::shared_ptr<BaseRule> GameSpec::createGlobalMessage(const nlohmann::json& currentRuleJson) {
    std::string value = currentRuleJson.at( SpecTags::VALUE);
    return std::make_shared<GameSpecification::GlobalMessage>(GameSpecification::GlobalMessage(value));
} 

std::shared_ptr<BaseRule> GameSpec::createInputChoice(const nlohmann::json& currentRuleJson){
    
    std::unique_ptr<StateValue> stateValuePointer ;
    std::string to = currentRuleJson.at( SpecTags::TO).get<std::string>();
    std::string prompt = currentRuleJson.at( SpecTags::PROMPT).get<std::string>();
    std::string result = currentRuleJson.at( SpecTags::RESULT).get<std::string>();
    double timeout = currentRuleJson.at( SpecTags::TIMEOUT).get<std::double_t >();

    auto choices = currentRuleJson.at( SpecTags::CHOICES);

    if(choices.is_array()) {
        std::vector<std::shared_ptr<StateValue>> listValue;
        createStateList(choices,stateValuePointer);//ptr is reference
    } 
    else if(choices.is_string()) {
        stateValuePointer = std::make_unique<StateValueString>(choices);
    } 
    else {
        //
        assert(false && "Input-Choice Rule needs to be Array or String");
    }
    
    return std::make_shared<GameSpecification::InputChoice>(
        GameSpecification::InputChoice(
            to,
            prompt,
            stateValuePointer,
            result,
            timeout
        )
    );
}

std::shared_ptr<BaseRule> GameSpec::createInputText(const nlohmann::json& currentRuleJson){
    std::unique_ptr<StateValue> stateValuePointer ;
    std::string user = currentRuleJson.at( SpecTags::TO).get<std::string>();
    std::string prompt = currentRuleJson.at( SpecTags::PROMPT).get<std::string>();
    std::string result = currentRuleJson.at( SpecTags::RESULT).get<std::string>();
    double timeout = currentRuleJson.at( SpecTags::TIMEOUT).get<std::double_t >();

    return std::make_shared<GameSpecification::InputText>(
            GameSpecification::InputText(
                    user,
                    prompt,
                    result,
                    timeout
            )
    );

}

std::shared_ptr<BaseRule> GameSpec::createInputVote(const nlohmann::json& currentRuleJson){
    std::unique_ptr<StateValue> stateValueChoices ;
    std::string to = currentRuleJson.at( SpecTags::TO).get<std::string>();
    std::string prompt = currentRuleJson.at( SpecTags::PROMPT).get<std::string>();
    std::string result = currentRuleJson.at( SpecTags::RESULT).get<std::string>();
    double timeout = currentRuleJson.at( SpecTags::TIMEOUT).get<std::double_t >();

    auto choices = currentRuleJson.at( SpecTags::CHOICES);

    if(choices.is_array()) {
        std::vector<std::shared_ptr<StateValue>> listValue;
        createStateList(choices,stateValueChoices);//ptr is reference
    }
    else if(choices.is_string()) {
        stateValueChoices = std::make_unique<StateValueString>(choices);
    }
    else {
        //
        assert(false && "Input-Choice Rule needs to be Array or String");
    }
    return std::make_shared<GameSpecification::InputChoice>(
            GameSpecification::InputChoice(
                    to,
                    prompt,
                    stateValueChoices,
                    result,
                    timeout
            )
    );

}

std::shared_ptr<BaseRule> GameSpec::createMessage(const nlohmann::json& currentRuleJson){
    auto to = currentRuleJson.at( SpecTags::TO);
    std::unique_ptr<StateValue> stateValuePointer ;
    if(to.is_array()) {
        std::vector<std::shared_ptr<StateValue>> listValue;
        createStateList(to,stateValuePointer);//ptr is reference
    }
    else if(to.is_string()) {
        stateValuePointer = std::make_unique<StateValueString>(to);
    }

    std::string value = currentRuleJson.at( SpecTags::VALUE).get<std::string>();
    return std::make_shared<GameSpecification::Message>(GameSpecification::Message(stateValuePointer,value));
}

std::shared_ptr<BaseRule> GameSpec::createReverse(const nlohmann::json& currentRuleJson){
    std::string list = currentRuleJson.at( SpecTags::LIST).get<std::string>();
    return std::make_shared<GameSpecification::Reverse>(GameSpecification::Reverse(list));
}

std::shared_ptr<BaseRule> GameSpec::createScores(const nlohmann::json& currentRuleJson){
    auto score = currentRuleJson.at(SpecTags::SCORE).get<std::string>();
    auto ascending = currentRuleJson.at(SpecTags::ASCENDING).get<std::string>();
    bool ascendingBool = (ascending == "true");
    return std::shared_ptr<BaseRule>(new Scores(score, ascendingBool));
}

std::shared_ptr<BaseRule> GameSpec::createShuffle(const nlohmann::json& currentRuleJson){
    std::string list = currentRuleJson.at( SpecTags::LIST).get<std::string>();
    return std::make_shared<GameSpecification::Shuffle>(GameSpecification::Shuffle(list));

}

std::shared_ptr<BaseRule> GameSpec::createSort(const nlohmann::json& currentRuleJson){
    std::string list = currentRuleJson.at( SpecTags::LIST).get<std::string>();
    std::string key = currentRuleJson.at( SpecTags::KEY).get<std::string>();
    return std::make_shared<GameSpecification::Sort>(GameSpecification::Sort(list,key));
}

