#include "include/Game.h"

#include <utility>

Game::Game(GameSpecification::GameSpec spec, GameState state) :
    gameSpec{std::move(spec)},
    gameState{std::move(state)},
    currentRuleIndex(0),
    messages{},
    isGameStarted(false)
{}

bool Game::isStarted() const {
    return isGameStarted;
}

void Game::startGame(const std::list<std::weak_ptr<User>>& players, const std::list<std::weak_ptr<User>>& audience) {
    gameState.startGame(players, audience);
    addMessageToAllSession(" The game has started!");
    importGameSpecRules();

    // Once the game has started, gameTick will end up being called.
    isGameStarted = true;
}

void Game::endGame() {
    addMessageToAllSession(" The game has ended!");
    isGameStarted = false;
}

void Game::addMessage(const UserId& userId, const std::string &message) noexcept {
    gameState.addMessage(userId, message);
}

std::list<std::pair<UserId, std::string>> Game::updateAndGetAllMessages() noexcept {
    return gameState.updateAndGetAllMessages();
}

void Game::gameTick() {
    if (!nextRules.empty() || currentRuleIndex < gameRules.size()) {
        if(nextRules.empty()){
            nextRules = {*std::next(gameRules.begin(), currentRuleIndex)};
            currentRuleIndex++;
        }
        
        std::list<std::shared_ptr<BaseRule>> newNextRules {};

        for(auto& rule : nextRules){
            rule->process(gameState);

            //probably can replace with std::insert
            for (auto& nextRule : rule->getNextPtr()){
                newNextRules.push_back(nextRule);
            }
        }

        nextRules = newNextRules;        
    } else {
        endGame();
    }
}

void Game::importGameSpecRules() {
    gameRules.clear();
    gameRules = gameSpec.getRules();
}

void Game::addMessageToAllPlayers(const std::string &message) noexcept {
    gameState.addMessageToAllPlayers(message);
}

void Game::addMessageToAllAudience(const std::string &message) noexcept {
    gameState.addMessageToAllAudience(message);
}

void Game::addMessageToAllSession(const std::string &message) noexcept {
    gameState.addMessageToEntireSession(message);
}
