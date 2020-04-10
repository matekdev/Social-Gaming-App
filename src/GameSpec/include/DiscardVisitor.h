#ifndef SOCIALGAMING_DISCARDVISITOR_H
#define SOCIALGAMING_DISCARDVISITOR_H

#include "GameStateVisitor.h"

// TODO Implement methods and add error handling
class DiscardVisitor : public GameStateVisitor {
public:
    DiscardVisitor(int num) : numberToDiscard(num) {};

    void visit(StateValueBoolean& stateValue) override {
        throw std::invalid_argument("Invalid State value, cannot discard");
    }

    void visit(StateValueNumber& stateValue) override {
        throw std::invalid_argument("Invalid State value, cannot discard");
    }

    void visit(StateValueString& stateValue) override {
        throw std::invalid_argument("Invalid State value, cannot discard");
    }

    void visit(StateValueList& stateValue) override {
        auto listToDiscard = stateValue.getList();
        listToDiscard.erase(listToDiscard.begin(), listToDiscard.begin() + numberToDiscard);
    }

    void visit(StateValueList& stateValue, StateValueList& list) override {
        throw std::invalid_argument("Invalid State value, cannot discard");
    }

    void visit(StateValueMap& stateValue) override {
        throw std::invalid_argument("Invalid State value, cannot discard");
    }
    void visit(StateValueNumber& valueTobeUpdated, StateValueNumber& value) override{

    }

private:
    int numberToDiscard = 0;
};


#endif //SOCIALGAMING_DISCARDVISITOR_H
