#ifndef SORT_H
#define SORT_H

#include "BaseRule.h"
#include <optional>
namespace GameSpecification{
class Sort : public BaseRule{
    public:
        Sort(const std::string& list, const std::string& _key);

       	void process(GameState&) override; 
    private:
        std::string list;
        std::optional<std::string> key;
};
}
#endif
