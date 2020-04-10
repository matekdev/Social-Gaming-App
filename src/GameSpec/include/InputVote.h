#ifndef INPUTVOTE_H
#define INPUTVOTE_H

#include "BaseRule.h"
namespace GameSpecification{
class InputVote : public BaseRule{
  	public:

		InputVote(const std::string& user,const std::string& prompt, const std::string& choices, const std::string& result, const double timeout);

    	void process(GameState&) override;
		//void parseRule(const nlohmann::json&) override;
	private:
		std::string user;
		std::string prompt;
		std::string choices;
		std::string result;
		double timeout;
};
}
#endif
