#include "StringEvaluation.h"
#include <iostream>
using map_pair = std::pair<std::string, std::shared_ptr<const StateValue>>;
using Map = std::unordered_map<std::string, std::shared_ptr<const StateValue>>;

static std::unordered_map<std::string, Operator> strToOp({
	{"==", Operator::EQUAL},
	{".", Operator::DOT},
	{"contains", Operator::CONTAINS},
	{"collects", Operator::COLLECTS},
	{"!", Operator::NEGATION},
	{"(", Operator::OPEN_PARENTHESIS},
	{"}", Operator::CLOSE_PARENTHESIS}
});

static std::unordered_map<Operator, std::string> opToStr({
	{Operator::EQUAL, "=="},
	{Operator::DOT, "."},
	{Operator::CONTAINS, "contains"},
	{Operator::COLLECTS, "collects"},
	{Operator::NEGATION, "!"},
	{Operator::OPEN_PARENTHESIS, "("},
	{Operator::CLOSE_PARENTHESIS, ")"}
});

int precedence(Operator op){
	switch(op){
		case Operator::EQUAL:
			return 1;
		case Operator::DOT:
		case Operator::CONTAINS:
		case Operator::COLLECTS:
			return 2;
		default:
			return 0;
	}
	
}



std::string applyOp(const std::string& a, const std::string& b, Operator op, GameState& gameState, Map& values){ 
	std::shared_ptr<const StateValue> val1 = nullptr;
	std::shared_ptr<const StateValue> val2 = nullptr;
	std::string opStr = opToStr[op];
	std::string strResult = a + opStr + b;
	//Get value for a and b in values
	if(values.find(a) != values.end()){
		val1 = values[a];	
	}else{
		//TODO: Add code to get value for a in gameState
		if(a == "players"){
			if(op == Operator::DOT){
				if(b == "size"){
					values.insert(map_pair(strResult, std::shared_ptr<const StateValue>(new StateValueNumber(gameState.getPlayerListSize()))));
					return strResult;
				}else{
					//TODO: Add code to handle the case when b is actually a field of perPlayerMap
				}
			}
		}else{
			auto gameStateVariable = gameState.getVariable(a);
			if(!gameStateVariable.has_value()){
				auto constantStateVar = gameState.getConstant(a);
				if(constantStateVar.has_value()){
					val1 = constantStateVar->lock();
				}else{//a is constant value
					try{
						//TODO: for now just assume the constant value will be integer
						int parsedInt = stoi(a);
						val1 = std::shared_ptr<const StateValue>(new StateValueNumber(parsedInt));

					}catch(std::invalid_argument& e){
						val1 = std::shared_ptr<const StateValue>(new StateValueString(a));
					}
					return strResult;
				}
			}else{
				val1 = gameStateVariable->lock();
			}
			
		}
	}
	if(b == "size"){
		auto result = static_cast<const StateValueList*>(val1.get())->getListConst().size();
		values.insert(map_pair(strResult, std::shared_ptr<const StateValue>(new StateValueNumber((int)result))));
		return strResult;
	}

	if(values.find(b) != values.end()){
		val2 = values[b];
	}else{
		//TODO: Add code to get value for b in gameState
		auto gameStateVariable = gameState.getVariable(a);
		if(gameStateVariable.has_value()){
			val2 = gameStateVariable->lock();
		}else{ //b is just a key string of a
			val2 = std::shared_ptr<const StateValue>(new StateValueString(b));
		}

	}
	switch(op){
		case Operator::EQUAL:
			{
				bool result;
				if(val1 == nullptr || val2 == nullptr)
					result = false;	
				else{
					/*
					result = val1->compare(val2);
					*/
					auto val1Casted = static_cast<const StateValueNumber*>(val1.get());
					auto val2Casted = static_cast<const StateValueNumber*>(val2.get());
					result = val1Casted->getValueConst() == val2Casted->getValueConst();
				}
				//Cache the result
				values.insert(map_pair(strResult, std::shared_ptr<const StateValue>(new StateValueBoolean(result))));
				break;
			}
		case Operator::DOT:
			{
				//TODO: Find value with key as val2 inside val1. 
				//val1 should be StateValueMap, val2 should be a string field
				auto val1Casted = static_cast<const StateValueMap*>(val1.get());
				auto val2Casted = static_cast<const StateValueString*>(val2.get());
				auto result = val1Casted->getValueConst(val2Casted->getValueConst()).value();

				//Cache the result in map: 
				values.insert(map_pair(strResult, std::shared_ptr<const StateValue>(result.lock())));
				break;
			}
		case Operator::CONTAINS:
			{
				bool result;
				if(val1 == nullptr || val2 == nullptr)
					result = false;
				//TODO: Add code here to handle "contains"
				values.insert(map_pair(strResult, std::shared_ptr<const StateValue>(new StateValueBoolean(result))));
				break;
			}
		case Operator::NEGATION:
			{
				bool result;
				if(val1 == nullptr)
					result = !(static_cast<const StateValueBoolean*>(val2.get())->getValueConst());
				else
					result = !(static_cast<const StateValueBoolean*>(val1.get())->getValueConst());
				//Add getNegationValue method for StateValueBoolean
				//result = static_cast<StateValueBoolean*>(val1)->getNegationValue();
				values.insert(map_pair(strResult, std::shared_ptr<const StateValue>(new StateValueBoolean(result))));
				break;
			}
		case Operator::COLLECTS:
			break;
		default:
			break;
	}
	return strResult;
}

void compute(std::stack<Operator>& ops, std::stack<std::string>& literalValues, GameState& gameState,
					std::unordered_map<std::string, std::shared_ptr<const StateValue>>& values){
		std::string str2 = literalValues.top();
        literalValues.pop(); 

        std::string str1 = "";
        if(!literalValues.empty()){
			str1 = literalValues.top();
        	literalValues.pop(); 
        }else{
        	str1 = str2; //str1 must always be an expression, eg: !a -> (str1 = a, str2 = "", op = !)
        	str2 = "";
        }

        Operator op = Operator::UNKNOWN;
        if(!ops.empty()){
        	op = ops.top(); 
        	ops.pop();
        }

       	literalValues.push(applyOp(str1, str2, op, gameState, values));
}

bool evaluate(GameState& gameState, const std::string& tokens){

	std::unordered_map<std::string, std::shared_ptr<const StateValue>> values;
	//std::vector<std::string> tokens;
	unsigned int i; 
      
    // stack to store string literal values. 
    std::stack<std::string> literalValues; 

      
    // stack to store operators. 
    std::stack<Operator> ops; 
    for(i = 0; i < tokens.length(); i++){ 
          
        // Current token is a whitespace, 
        // skip it. 
        if(tokens[i] == ' ') 
            continue; 
          
        // Current token is an opening  
        // brace, push it to 'ops' 
        else if(tokens[i] == '('){ 
            ops.push(Operator::OPEN_PARENTHESIS); 
        } 
          
        // Current token is a aphabetic letter or digit, push  
        // it to stack for strings 
        else if(isalpha(tokens[i]) || isdigit(tokens[i])){ 
            std::string val = ""; 
              
            while(i < tokens.length() &&  
                        (isalpha(tokens[i]) || isdigit(tokens[i]))) 
			{ 
				val += tokens[i];
                i++; 
            } 
            i--;
			//In case of literal number, cache the value
			try{
				int numVal = stoi(val);
				values.insert(map_pair(val, std::shared_ptr<const StateValue>(new StateValueNumber(numVal))));
			}catch(std::invalid_argument& e){

			}

			//Treat ".contains" as one operator "contains"
            if(val == "contains"){
				ops.pop(); //pop the "." out
				ops.push(Operator::CONTAINS);
			}else{
            	literalValues.push(val); 
			}
        } 
          
        // Closing brace encountered, solve  
        // entire brace. 
        else if(tokens[i] == ')') 
        { 
            while(!ops.empty() && ops.top() != Operator::OPEN_PARENTHESIS) 
            { 
				compute(ops, literalValues, gameState, values);
            } 
              
            // pop opening brace. 
            if(!ops.empty()) 
               ops.pop(); 
        }
          
        // Current token is an operator. 
        else
        { 
            // While top of 'ops' has same or greater  
            // precedence to current token, which 
            // is an operator. Apply operator on top  
            // of 'ops' to top two elements in values stack.
            std::string opStr(1, tokens[i]); 
			Operator currentOp = strToOp[opStr];
			if(tokens[i] == '='){
				currentOp = strToOp["=="];
				i+=2;
			}
            while(!ops.empty() && precedence(ops.top()) 
                                >= precedence(currentOp)){ 
				compute(ops, literalValues, gameState, values);
            } 
              
            // Push current token to 'ops'. 
			ops.push(currentOp);
        } 
    } 
      
    // Entire expression has been parsed at this 
    // point, apply remaining ops to remaining 
    // values. 
    while(!ops.empty()){ 
		compute(ops, literalValues, gameState, values);
    } 
      
    // Top of 'values' contains result, return it. 
    return static_cast<const StateValueBoolean*>(values[literalValues.top()].get())->getValueConst(); 	

}


