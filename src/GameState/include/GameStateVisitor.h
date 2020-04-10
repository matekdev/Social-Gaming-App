#ifndef SOCIALGAMING_GAMESTATEVISITOR_H
#define SOCIALGAMING_GAMESTATEVISITOR_H

#include "StateValue.h"
#include <StateValueBoolean.h>
#include <StateValueNumber.h>
#include <StateValueString.h>
#include <StateValueList.h>
#include <StateValueMap.h>

class GameStateVisitor {
public:
    virtual void visit(StateValueBoolean& stateValue) = 0;
    virtual void visit(StateValueNumber& stateValue) = 0;
    virtual void visit(StateValueString& stateValue) = 0;
    virtual void visit(StateValueList& stateValue) = 0;
    virtual void visit(StateValueList& stateValue, StateValueList& list) = 0;
    virtual void visit(StateValueMap& stateValue) = 0;
    virtual void visit(StateValueNumber& valueTobeUpdated, StateValueNumber& value) = 0;
};

#endif
