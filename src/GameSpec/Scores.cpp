#include "Scores.h"

#include <utility>

using GameSpecification::Scores;
using json = nlohmann::json;

Scores::Scores(std::string  scoreValue, bool ascending) :
        BaseRule({}),
        score(std::move(scoreValue)),
        ascending(ascending)
        {}

void Scores::process(GameState& gameState){
    auto optionalValue = gameState.getPerPlayerValue(score);
    if (optionalValue.has_value()) {
        auto listOfValuePairs = optionalValue.value().get();

        std::vector<std::pair<std::string, int>> nameAndValues;
        for (const auto& valuePair : listOfValuePairs) {
            auto username = valuePair.user.lock()->getUserNameValue();
            auto valuePtr = static_cast<StateValueNumber*>(valuePair.value.get());
            double value = (valuePtr != nullptr) ? valuePtr->getValue() : -1;
            nameAndValues.emplace_back(std::make_pair<>(username, value));
        }

        bool asc = ascending;
        std::sort(nameAndValues.begin(), nameAndValues.end(), [asc](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
           if (asc) {
               return a.second > b.second;
           } else {
               return a.second < b.second;
           }
        });

        std::string response;
        for (const auto& nameAndValue : nameAndValues) {
            response.append(nameAndValue.first + " had " + std::to_string(nameAndValue.second) + " " + score + "\n");
        }
        gameState.addMessageToAllPlayers(response);
    }
}