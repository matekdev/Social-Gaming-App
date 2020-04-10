#include "GameState.h"
GameState::GameState() :
    gameStarted(false)
    {}

void GameState::startGame(const std::list<std::weak_ptr<User>>& playerList, const std::list<std::weak_ptr<User>>& audienceList) {
    gameStarted = true;
    this->playerList = playerList;
    this->audienceList = audienceList;
    initializePerPlayerMap();
    initializePerAudienceMap();
}

void GameState::endGame() {
    gameStarted = false;
}

bool GameState::isGameStarted() const {
    return gameStarted;
}

std::optional<std::weak_ptr<const StateValue>> GameState::getConstant(const std::string& key) {
    auto it = constantsMap.find(key);
    return (it != constantsMap.end()) ?
        std::make_optional(std::weak_ptr(it->second)) :
           std::nullopt;
}

std::optional<std::weak_ptr<StateValue>> GameState::getVariable(const std::string& key) {
    auto it = variablesMap.find(key);
    return (it != variablesMap.end()) ?
           std::make_optional(std::weak_ptr(it->second)) :
           std::nullopt;
}

std::optional<std::weak_ptr<const StateValue>> GameState::getConstantOrVariable(const std::string &key) {
    auto constantValue = getConstant(key);
    if (constantValue.has_value()){
        return constantValue;
    }
    return getVariable(key);
}

std::optional<std::reference_wrapper<std::vector<GameState::StateValueUserPair>>>
        GameState::getPerPlayerValue(const std::string& key) {
    auto it = perPlayerMap.find(key);
    return (it != perPlayerMap.end()) ?
           std::make_optional(std::reference_wrapper(it->second)) :
           std::nullopt;
}

std::optional<std::reference_wrapper<std::vector<GameState::StateValueUserPair>>>
        GameState::getPerAudienceValue(const std::string& key) {
    auto it = perPlayerMap.find(key);
    return (it != perPlayerMap.end()) ?
           std::make_optional(std::reference_wrapper(it->second)) :
           std::nullopt;
}

std::optional<std::reference_wrapper<std::vector<GameState::StateValueUserPair>>>
GameState::getPerUserValue(const std::string& key) {
    auto perPlayerValue = getPerPlayerValue(key);
    if (perPlayerValue.has_value()){
        return perPlayerValue;
    }
    return getPerAudienceValue(key);
}

void GameState::addValue(const std::string &key, StateValueBoolean value, const ValueType& valueType) {
    auto valuePtr = std::make_shared<StateValueBoolean>(std::move(value));
    std::pair<std::string, std::shared_ptr<StateValue>> pair = std::make_pair(key, valuePtr);
    insertIntoCorrectMap(valueType, pair);
}

void GameState::addValue(const std::string &key, StateValueNumber value, const ValueType& valueType) {
    auto valuePtr = std::make_shared<StateValueNumber>(std::move(value));
    std::pair<std::string, std::shared_ptr<StateValue>> pair = std::make_pair(key, valuePtr);
    insertIntoCorrectMap(valueType, pair);
}

void GameState::addValue(const std::string &key, StateValueString value, const ValueType& valueType) {
    auto valuePtr = std::make_shared<StateValueString>(std::move(value));
    std::pair<std::string, std::shared_ptr<StateValue>> pair = std::make_pair(key, valuePtr);
    insertIntoCorrectMap(valueType, pair);
}

void GameState::addValue(const std::string &key, StateValueList value, const ValueType& valueType) {
    auto valuePtr = std::make_shared<StateValueList>(std::move(value));
    std::pair<std::string, std::shared_ptr<StateValue>> pair = std::make_pair(key, valuePtr);
    insertIntoCorrectMap(valueType, pair);
}

void GameState::addValue(const std::string &key, StateValueMap value, const ValueType& valueType) {
    auto valuePtr = std::make_shared<StateValueMap>(std::move(value));
    std::pair<std::string, std::shared_ptr<StateValue>> pair = std::make_pair(key, valuePtr);
    insertIntoCorrectMap(valueType, pair);
}

void GameState::insertIntoCorrectMap(const GameState::ValueType &valueType,
                                     std::pair<std::string, std::shared_ptr<StateValue>>& pair) {
    switch (valueType){
        case CONSTANT:
            constantsMap.insert(std::move(pair));
            break;
        case VARIABLE:
            variablesMap.insert(std::move(pair));
            break;
        case PER_PLAYER:
            perPlayerInitialMap.insert(std::move(pair));
            break;
        case PER_AUDIENCE:
            perAudienceInitialMap.insert(std::move(pair));
            break;
    }
}

void GameState::addMessage(const UserId& userId, const std::string &message) noexcept{
    messages.emplace_back(userId, message);
}

void GameState::clearMessages() noexcept {
    messages.clear();
}

std::list<std::pair<UserId, std::string>> GameState::updateAndGetAllMessages() noexcept{
    auto gameMessages = messages;
    clearMessages();

    return gameMessages;
}

void GameState::addConfig(const GameConfig& gameConfig){
    this->gameConfig = gameConfig;
}

void GameState::initializePerPlayerMap() {
    for (const auto& value : perPlayerInitialMap){
        std::string key = value.first;
        perPlayerMap[key] = std::vector<StateValueUserPair>();
        for (const auto& user : playerList) {
            perPlayerMap[key].push_back(StateValueUserPair{user, value.second});
        }
    }
    // Input names into perPlayer map
    perPlayerMap["name"] = std::vector<StateValueUserPair>();
    for (const auto& value : playerList){
        const auto& user = value.lock();
        perPlayerMap["name"].push_back(StateValueUserPair{user,std::make_shared<StateValueString>(StateValueString{user->getUserNameValue()})});
    }
    perPlayerInitialMap.clear();
}

void GameState::initializePerAudienceMap() {
    for (const auto& value : perAudienceInitialMap){
        std::string key = value.first;
        perAudienceMap[key] = std::vector<StateValueUserPair>();
        for (const auto& user : audienceList){
            perAudienceMap[key].push_back(StateValueUserPair{user, value.second});
        }
    }
    // Input names into perAudience map
    perAudienceMap["name"] = std::vector<StateValueUserPair>();
    for (const auto& value : audienceList){
        const auto& user = value.lock();
        perAudienceMap["name"].push_back(StateValueUserPair{user,std::make_shared<StateValueString>(StateValueString{user->getUserNameValue()})});
    }
    perAudienceInitialMap.clear();
}

void GameState::addMessageToAllPlayers(const std::string &message) noexcept {
    for (const auto& playerPtr : playerList){
        if (auto player = playerPtr.lock()){
            addMessage(player->getUserId(), message);
        }
    }
}

void GameState::addMessageToAllAudience(const std::string &message) noexcept {
    for (const auto& audiencePtr : audienceList){
        if (auto audience = audiencePtr.lock()){
            addMessage(audience->getUserId(), message);
        }
    }
}

void GameState::addMessageToEntireSession(const std::string &message) noexcept {
    addMessageToAllAudience(message);
    addMessageToAllPlayers(message);
}

std::list<std::weak_ptr<User>> &GameState::getPlayerList() {
    return playerList;
}

std::list<std::weak_ptr<User>> &GameState::getAudienceList() {
    return audienceList;
}
