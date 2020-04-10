#ifndef INPUTCHOICERULE_H
#define INPUTCHOICERULE_H

#include "GameSpec.h"

namespace GameSpecification{

	class InputChoice: public BaseRule{
		public:
			InputChoice(std::string , std::string , std::unique_ptr<StateValue>&, std::string , double);
			void process(GameState&) override; 

		private:
			std::string to;	//represents field "to" in input choice rule
			std::string prompt; 
			std::unique_ptr<StateValue> choices;
			std::string result; //represents field "result" in input choice rule
			std::optional<double> timeout;
			
	};

}

#endif
