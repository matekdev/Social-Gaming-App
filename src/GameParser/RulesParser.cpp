#include <list>
#include <nlohmann/json.hpp>
#include <cassert>
#include <fstream>
#include <string>
#include "BaseRule.h"
#include "Add.h"
#include "Deal.h"
#include "GlobalMessage.h"
#include "IncludeChoice.h"
#include "IncludeText.h"
#include "InputVote.h"
#include "Message.h"
#include "Reverse.h"
#include "Scores.h"
#include "Sort.h"
#include <iostream>

std::vector<GameSpecification::BaseRule> jsonToGameSpec (nlohmann::json jsonRules){
    std::vector<GameSpecification::BaseRule> rules;
    for (auto& element : jsonRules){
        if(element["rule"] == "foreach"){
            
        }
        else if (element["rule"] == "loop"){

        }
        else if (element["rule"] == "inparallel"){

        }
        else if (element["rule"] == "parallelfor"){

        }
        else if (element["rule"] == "timer"){

        }
        else if (element["rule"] == "add"){
            std::string to = element["to"];
            SpecValue value = element["value"];
            GameSpecification::Add add {to, value};
            rules.push_back(add);
        }
        else if(element["rule"] == "deal"){
            std::string from = element["from"];
            SpecValue to = element["to"];
            int count = element["count"];
            GameSpecification::Deal deal{from, to, count};
            rules.push_back(deal);
        }
        else if(element["rule"] == "discard"){
            std::string from = element["from"];
            int count = element["count"];
            GameSpecification::Discard discard{from, count};
            rules.push_back(discard);
        }
        else if(element["rule"] == "extend"){
            std::string target = element["target"];
            SpecValue list = element["list"];
            GameSpecification::Extend extend{target, list};
            rules.add(extend);
        }
        else if(element["rule"] == "global-message"){
            std::string value = element["value"];
            GameSpecification::GlobalMessage globalMessage {value};
            rules.push_back(globalMessage);
        }
        else if(element["rule"] == "input-choice"){
                std::string to = element["to"];
    			Message prompt = element["prompt"]; 
    			SpecValue choices = element["choices"];
    			std::string result = element["result"];
    			std::string timeout = element["timeout"];
                GameSpecification::InputChoice inputChoice {to, prompt, choices, result, timeout};
                rules.push_back(inputChoice);
        }
        else if(element["rule"] == "input-text"){
            std::string user = element["to"];
            std::string prompt = element["prompt"];
            std::string result = element["result"];
            double timeout = element["timeout"];
            GameSpecification::InputText inputText {user, prompt, result, timeout};
            rules.push_back(inputText);

        }
        else if(element["rule"] == "input-vote"){
            std::string user = element["to"];
            std::string prompt = element["prompt"];
            std::string choices = element["choices"];
            std::string result = element["result"];
            double timeout = element["timeout"];
            GameSpecification::InputVote inputVote {user, prompt, choices, result, timeout};
            rules.push_back(inputVote);
        }
        else if(element["rule"] == "message"){
            SpecValue to = element["to"];
			std::string	messValue = element["value"];
            GameSpecification::Message message{to, value};
            rules.push_back(message);
        }
        else if(element["rule"] == "reverse"){
            std::string list = element["list"];
            GameSpecification::Reverse reverse{list};
            rules.push_back(reverse);
        }
        else if(element["rule"] == "scores"){
            double score = element["score"];
            bool ascending = element["ascending"];
            GameSpecification::Scores scores{score, ascending};
            rules.push_back(scores);
        }
        else if(element["rule"] == "shuffle"){

        }
        else if(element["rule"] == "sort"){
            std::string list = element["sort"];
            std::string key = element["key"];
            GameSpecification::Sort sort{list, key};
            rules.push_back(sort);
        }
        return rules;
    }
}

//-------------------
//Lam's Parsing Code
//-------------------

/*
void Add::parseRule(const json& ruleJson){
    try{
        to = ruleJson
            .at("to")
            .get<std::string>();
            
    }catch(json::exception &e){
        std::cout << e.what() << "\n";
    }
}

void Deal::parseRule(const json& ruleJson){
    try{
        from = ruleJson.at("from").get<std::string>();
        count = ruleJson.at("count").get<int>();
        json to = ruleJson.at("to");
        if(to.is_string()){
            this->to.value = to.get<std::string>();
        }else{
            this->to.value = to.get<std::vector<std::string>>();
        }
        
    }catch(json::exception &e){
        //TODO: Handle exception more properly
        std::cout << e.what() << "\n";
    }
}

void Discard::parseRule(const json& ruleJson){
    try{
       this->from = ruleJson.at("from").get<std::string>();
       this->count = ruleJson.at("count").get<int>();
    }catch(json::exception& e){
        //TODO: Handle exception more properly
        std::cout << e.what() << "\n";
    }
}

void Extend::parseRule(const json& ruleJson){
    try{
        target = ruleJson.at("target").get<std::string>();
        // json list = ruleJson.at("list");
        // if(list.is_string()){
        //     this->list.value = list.get<std::string>();
        // }else{
        //     this->list.value = list.get<std::vector<std::string>>();
        // }
    }catch(json::exception &e){
        //TODO: Handle exception more properly
        std::cout << e.what() << "\n";
    }
}

void ForEach::parseRule(const json &ruleJson) {
	
    try{
		element = ruleJson.at("element").get<std::string>();
		auto listJson = ruleJson.at("list");
		if(listJson.is_array()){
			list.value = listJson.get<std::vector<std::string>>();
		}else{
			list.value = listJson.get<std::string>();
		}
		auto subRulesJson = ruleJson.at("rules");
		for(auto& it : subRulesJson.items()){
			auto rule = it.value();
			auto ruleType = stringToRuleType.at(rule.at("rule").get<std::string>());
			auto baseRulePtr = getRulePtrFromRuleType(ruleType);
			if(!baseRulePtr){
				switch(ruleType){
					case RuleType::ForEachType:
						baseRulePtr = std::shared_ptr<BaseRule>(new ForEach());
						break;
					case RuleType::InparallelType:
						baseRulePtr = std::shared_ptr<BaseRule>(new Inparallel());
						break;
					case RuleType::LoopType:
						baseRulePtr = std::shared_ptr<BaseRule>(new Loop());
						break;
					case RuleType::ParallelforType:
						baseRulePtr = std::shared_ptr<BaseRule>(new Parallelfor());
						break;
					default:
						assert(false);
				}
			}
			baseRulePtr->parseRule(rule);
			baseRulePtr->setParentPtr(this->getPtr());
			this->subRules.push_back(baseRulePtr);
		}
		//Set next pointer for each subRule
		for(auto it = subRules.begin(); it != subRules.end() - 1; ++it){
			(*it)->setNextPtr(*(it+1));
		}
		//Set next pointer for the last rule to point back to the parent Rule
		(*subRules.rbegin())->setNextPtr((this->getPtr()));	
    }catch(json::exception &e){
        //TODO: Handle exception more properly
        std::cout << e.what() << "\n";
    }
	
}

void GlobalMessage::parseRule(const json &ruleJson){
    try{
       value = ruleJson.at("value").get<std::string>(); 
    }catch(json::exception &e){
        //TODO: Handle exception more properly
        std::cout << e.what() << "\n";
    }
}

void Inparallel::parseRule(const json &ruleJson){
	try{

	}catch(json::exception &e){
		//TODO: Handle exception more properly
		std::cout << e.what() << "\n";
	}
}

void InputChoice::parseRule(const json &ruleJson){
	try{
		to = ruleJson.at("to").get<std::string>();
		result = ruleJson.at("result").get<std::string>();
		if(ruleJson.find("timeout") != ruleJson.end())
			timeout = std::optional<double>{ruleJson.at("timeout").get<double>()};
		json choices = ruleJson.at("choices");
		if(choices.is_string()){
			this->choices.value = choices.get<std::string>();
		}else{
			this->choices.value = choices.get<std::vector<std::string>>();
		}
		prompt.parseRule(ruleJson.at("prompt"));
	}catch(json::exception &e){
		//TODO: Handle exception more properly
		std::cout << e.what() << "\n";
	}
}

void InputText::parseRule(const json &ruleJson){
    try{
        user = ruleJson.at("to").get<std::string>();
        prompt = ruleJson.at("prompt").get<std::string>();
        result = ruleJson.at("result").get<std::string>();
        if(ruleJson.find("timeout") != ruleJson.end())
            timeout = ruleJson.at("timeout").get<double>();
    }catch(json::exception &e){
        //TODO: Handle exception more properly
        std::cout << e.what() << "\n";
    }
}

void InputVote::parseRule(const json &ruleJson){
    try{
        user = ruleJson.at("user").get<std::string>();
        prompt = ruleJson.at("prompt").get<std::string>();
        choices = ruleJson.at("choices").get<std::string>();
        result = ruleJson.at("result").get<std::string>();
        if(ruleJson.find("timeout") != ruleJson.end())
            timeout = ruleJson.at("timeout").get<double>();
    }catch(json::exception &e){
        //TODO: Handle exception more properly
        std::cout << e.what() << "\n";
    }
}

void Loop::parseRule(const json &ruleJson){
    try{

    }catch(json::exception &e){
        //TODO: Handle exception more properly
        std::cout << e.what() << "\n";
    }
}

void Message::parseRule(const json &ruleJson){
	try{
		messValue = ruleJson.at("value").get<std::string>();
		json to = ruleJson.at("to");
		if(to.is_string()){
			this->to.value = to.get<std::string>();
		}else{
			this->to.value = to.get<std::vector<std::string>>();
		}
	}catch(json::exception& e){
		//TODO: Handle exception more properly
		std::cout << e.what() << "\n";
	}
}

void Parallelfor::parseRule(const json &ruleJson){
    try{

    }catch(json::exception &e){
        //TODO: Handle exception more properly
        std::cout << e.what() << "\n";
    }
}

void Reverse::parseRule(const json& ruleJson){
    try{
        list = ruleJson.at("list").get<std::string>();
    }catch(json::exception &e){
        //TODO: Handle exception more properly
        std::cout << e.what() << "\n";
    }
}

void Scores::parseRule(const json &ruleJson){
    try{
        score = ruleJson.at("score").get<int>();
        ascending = ruleJson.at("ascending").get<bool>();
    }catch(json::exception &e){
        //TODO: Handle exception more properly
        std::cout << e.what() << "\n";
    }
}

void Shuffle::parseRule(const json &ruleJson){
    try{
        list = ruleJson.at("list").get<std::string>();
    }catch(json::exception &e){
        //TODO: Handle exception more properly
        std::cout << e.what() << "\n";
    }
}

void Sort::parseRule(const json &ruleJson){
    try{
        list = ruleJson.at("list").get<std::string>();
        if(ruleJson.find("key") != ruleJson.end())
            key = std::optional<std::string>{ruleJson.at("key").get<std::string>()};
    }catch(json::exception &e){
        //TODO: Handle exception more properly
        std::cout << e.what() << "\n";
    }
}

void Switch::parseRule(const json &ruleJson) override{
    
}
void Timer::parseRule(const json &ruleJson){
    try{

    }catch(json::exception &e){
        //TODO: Handle exception more properly
        std::cout << e.what() << "\n";
    }
}

void When::parseRule(const json &ruleJson) override{

}
*/