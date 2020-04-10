#ifndef INPUTTEXT_H
#define INPUTTEXT_H

#include "BaseRule.h"

namespace GameSpecification{
class InputText : public BaseRule{
    public:
		InputText(const std::string& user, const std::string& prompt, const std::string& result, const double timeout);

		void process(GameState&) override;

    private:
        std::string user;
        std::string prompt;
        std::string result;
        double timeout;
};
}
#endif
