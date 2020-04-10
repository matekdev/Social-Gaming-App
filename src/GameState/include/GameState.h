#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <utility>
#include <boost/variant.hpp>
#include "User.h"
#include "GameConfig.h"
#include "StateValue.h"
#include "StateValueBoolean.h"
#include "StateValueNumber.h"
#include "StateValueString.h"
#include "StateValueList.h"
#include "StateValueMap.h"
#include <optional>

class GameState {
public:
    GameState();
    bool isGameStarted() const;
    void startGame(const std::list<std::weak_ptr<User>>& playerList, const std::list<std::weak_ptr<User>>& audienceList);
    void endGame();
    enum ValueType {
        CONSTANT,
        VARIABLE,
        PER_PLAYER,
        PER_AUDIENCE
    };

    struct StateValueUserPair{
        std::weak_ptr<User> user;
        std::shared_ptr<StateValue> value;
    };

    std::optional<std::weak_ptr<const StateValue>> getConstant(const std::string& key);
    std::optional<std::weak_ptr<const StateValue>> getConstantOrVariable(const std::string& key);
    std::optional<std::weak_ptr<StateValue>> getVariable(const std::string& key);

    std::optional<std::reference_wrapper<std::vector<GameState::StateValueUserPair>>>
    getPerPlayerValue(const std::string& key);

    std::optional<std::reference_wrapper<std::vector<GameState::StateValueUserPair>>>
    getPerAudienceValue(const std::string& key);

    int getPlayerListSize() const {
        return playerList.size();
    }

    std::optional<std::reference_wrapper<std::vector<GameState::StateValueUserPair>>>
    getPerUserValue(const std::string& key);

    void addConfig(const GameConfig& config);

    // These methods should only be called when GameState is begin constructed in the parser
    void addValue(const std::string& key, StateValueBoolean value, const ValueType& valueType);
    void addValue(const std::string &key, StateValueNumber value, const ValueType& valueType);
    void addValue(const std::string &key, StateValueString value, const ValueType& valueType);
    void addValue(const std::string &key, StateValueList value, const ValueType& valueType);
    void addValue(const std::string &key, StateValueMap value, const ValueType& valueType);

    void addMessage(const UserId&, const std::string& message) noexcept;
    void addMessageToAllPlayers(const std::string& message) noexcept;
    void addMessageToAllAudience(const std::string& message) noexcept;
    void addMessageToEntireSession(const std::string& message) noexcept;
    void clearMessages() noexcept;
    std::list<std::weak_ptr<User>>& getPlayerList();
    std::list<std::weak_ptr<User>>& getAudienceList();
    std::list<std::pair<UserId, std::string>> updateAndGetAllMessages() noexcept;
private:
    bool gameStarted;
    std::unordered_map<std::string, std::shared_ptr<const StateValue>> constantsMap;
    std::unordered_map<std::string, std::shared_ptr<StateValue>> variablesMap;

    // These maps set up the initial values for the per-player/per-audience maps
    std::unordered_map<std::string, std::shared_ptr<StateValue>> perPlayerInitialMap;
    std::unordered_map<std::string, std::shared_ptr<StateValue>> perAudienceInitialMap;
    
    std::unordered_map<std::string, std::vector<StateValueUserPair>> perPlayerMap;
    std::unordered_map<std::string, std::vector<StateValueUserPair>> perAudienceMap;
    GameConfig gameConfig;
    std::list<std::pair<UserId, std::string>> messages;
    std::list<std::weak_ptr<User>> playerList;
    std::list<std::weak_ptr<User>> audienceList;
    void insertIntoCorrectMap(const GameState::ValueType &valueType,
                              std::pair<std::string, std::shared_ptr<StateValue>>& pair);
    void initializePerPlayerMap();
    void initializePerAudienceMap();
};

#endif
