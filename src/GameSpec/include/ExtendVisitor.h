#ifndef SOCIALGAMING_EXTENDVISITOR_H
#define SOCIALGAMING_EXTENDVISITOR_H

#include "GameStateVisitor.h"

// TODO Implement methods and add error handling
class ExtendVisitor : public GameStateVisitor {
public:
    ExtendVisitor(){

    }

    void visit(StateValueBoolean& stateValue) override {
        throw std::invalid_argument("Invalid State value cannot extend");
    }

    void visit(StateValueNumber& stateValue) override {
        throw std::invalid_argument("Invalid State value cannot extend");
    }

    void visit(StateValueString& stateValue) override {
        throw std::invalid_argument("Invalid State value cannot extend");
    }

    void visit(StateValueList& stateValue) override {
        throw std::invalid_argument("Invalid State value, cannot extend");
    }

    void visit(StateValueList& stateValue, StateValueList& value) override {
        auto target = stateValue.getList();
        auto list = value.getList();
        target.insert(target.end(), list.begin(), list.end());
    }

    void visit(StateValueMap& stateValue) override {
        throw std::invalid_argument("Invalid State value cannot extend");
    }
    void visit(StateValueNumber& valueTobeUpdated, StateValueNumber& value) override{

    }

};
#endif //SOCIALGAMING_EXTENDVISITOR_H
