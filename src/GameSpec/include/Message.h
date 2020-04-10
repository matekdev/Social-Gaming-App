#ifndef MESSAGERULE_H
#define MESSAGERULE_H

#include "BaseRule.h"


namespace GameSpecification{
	class Message: public BaseRule{
		public:
			Message(std::unique_ptr<StateValue>&, std::string );

			void process(GameState&) override;
		private:
			std::unique_ptr<StateValue> to;
			std::string	messValue;
	};
}


#endif
