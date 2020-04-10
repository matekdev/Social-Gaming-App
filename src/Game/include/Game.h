#ifndef GAME_H
#define GAME_H

#include "GameState.h"
#include "GameSpec.h"
#include <stack>

class Game {
public:
    Game(GameSpecification::GameSpec spec, GameState state);
    bool isStarted() const;
    void startGame(const std::list<std::weak_ptr<User>>& players, const std::list<std::weak_ptr<User>>& audience);
    void endGame();
    void addMessage(const UserId& userId, const std::string &message) noexcept;
    void addMessageToAllPlayers(const std::string& message) noexcept;
    void addMessageToAllAudience(const std::string& message) noexcept;
    void addMessageToAllSession(const std::string& message) noexcept;
    std::list<std::pair<UserId, std::string>> updateAndGetAllMessages() noexcept;
    void gameTick();


private:
    // Rule Processor Methods
    void importGameSpecRules();

    GameSpecification::GameSpec gameSpec;
    GameState gameState;
    std::list<std::shared_ptr<BaseRule>> gameRules;
    unsigned int currentRuleIndex;

    std::list<std::shared_ptr<BaseRule>> nextRules;

    std::list<std::weak_ptr<User>> playerList;
    std::list<std::weak_ptr<User>> audienceList;
    std::list<std::string> messages;
    bool isGameStarted;
};

#endif //GAME_H
