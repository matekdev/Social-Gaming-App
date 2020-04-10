//
// Created by josh on 2020-03-19.
//

#ifndef SOCIALGAMING_MESSAGEVISITOR_H
#define SOCIALGAMING_MESSAGEVISITOR_H

#include "GameStateVisitor.h"

// TODO Implement methods and add error handling
class MessageVisitor : public GameStateVisitor {
public:
    MessageVisitor(){

    }

    void visit(StateValueBoolean& stateValue) override {
        return;
    }

    void visit(StateValueNumber& stateValue) override {
        return;
    }

    void visit(StateValueString& stateValue) override {
        return;
    }

    void visit(StateValueList& stateValue) override {
        return;
    }

    void visit(StateValueList& stateValue, StateValueList& value) override {
        return;
    }

    void visit(StateValueMap& stateValue) override {
        return;
    }

    void visit(StateValueNumber& valueTobeUpdated, StateValueNumber& value) override{

    }

};

#endif //SOCIALGAMING_MESSAGEVISITOR_H
