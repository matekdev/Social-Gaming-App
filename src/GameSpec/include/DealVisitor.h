#ifndef SOCIALGAMING_DEALVISITOR_H
#define SOCIALGAMING_DEALVISITOR_H

#include "GameStateVisitor.h"

// TODO Implement methods and add error handling
class DealVisitor : public GameStateVisitor {
public:
    DealVisitor(int num, StateValueList& list) : numberToDeal(num), dealtToList(list){};

    void visit(StateValueBoolean& stateValue) override {
        throw std::invalid_argument("Invalid State value, cannot deal");
    }

    void visit(StateValueNumber& stateValue) override {
        throw std::invalid_argument("Invalid State value, cannot deal");
    }

    void visit(StateValueString& stateValue) override {
        throw std::invalid_argument("Invalid State value, cannot deal");
    }

    void visit(StateValueList& stateValue) override {
        // TODO : Complete implementation for list attributes
        auto listValue = stateValue.getList();
        auto targetList = dealtToList.getList();

        targetList.insert(targetList.end(), std::make_move_iterator(listValue.begin()), 
            std::make_move_iterator(listValue.begin()) + numberToDeal);
    }

    void visit(StateValueMap& stateValue) override {
        throw std::invalid_argument("Invalid State value, cannot deal");
    }

    void visit(StateValueNumber& valueTobeUpdated, StateValueNumber& value) override{

    }

private:
    int numberToDeal = 0;
    StateValueList& dealtToList;
};


#endif //SOCIALGAMING_DEALVISITOR_H
