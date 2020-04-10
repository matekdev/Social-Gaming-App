#ifndef SWITCH_H
#define SWITCH_H

#include "BaseRule.h"
namespace GameSpecification{
    class SwitchCase{
        public:
            SwitchCase(const std::string& value, const std::list<std::shared_ptr<BaseRule>>&);

            //more functionality required
        private:
            std::string value;
            std::list<std::shared_ptr<BaseRule>> rules;
    };

    class Switch : public BaseRule{
        public:
            Switch(const std::string& value, const std::string& listName, const std::vector<SwitchCase>& cases);
            void process(GameState&) override;
        private:
            std::string value;
            std::string listName;
            std::vector<SwitchCase> cases;
    };
}
#endif
