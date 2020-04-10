#ifndef SOCIALGAMING_SHUFFLEVISITOR_H
#define SOCIALGAMING_SHUFFLEVISITOR_H

#include "GameStateVisitor.h"
#include <algorithm>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm/random_shuffle.hpp>
#include <random>

// TODO Implement methods and add error handling
class ShuffleVisitor : public GameStateVisitor {
public:
    ShuffleVisitor(){

    }

    void visit(StateValueBoolean& stateValue) override {
        throw std::invalid_argument("Invalid State value cannot shuffle");
    }

    void visit(StateValueNumber& stateValue) override {
        throw std::invalid_argument("Invalid State value cannot shuffle");
    }

    void visit(StateValueString& stateValue) override {
        throw std::invalid_argument("Invalid State value cannot shuffle");
    }

    void visit(StateValueList& stateValue) override {
        unsigned seed = 0;
        std::shuffle(stateValue.getList().begin(), stateValue.getList().end(), std::default_random_engine(seed));
    }

    void visit(StateValueList& stateValue, StateValueList& value) override {
        throw std::invalid_argument("Invalid State value cannot shuffle");
    }

    void visit(StateValueMap& stateValue) override {
        auto a = stateValue.getMap();
        unsigned seed = 0;
        std::vector<std::pair<std::string, std::shared_ptr<StateValue>>> elems(a.begin(), a.end());
        std::shuffle(elems.begin(), elems.end(), std::default_random_engine(seed));
    }

    void visit(StateValueNumber& valueTobeUpdated, StateValueNumber& value) override{

    }

};

#endif //SOCIALGAMING_SHUFFLEVISITOR_H
