#include "include/GameConfig.h"

using Configuration::PlayerCount;

GameConfig::GameConfig()
    : name(""),
      playerCount({0, 0}), 
      audience(false),
      setup({}){
}

GameConfig::GameConfig(const std::string& name)
    : GameConfig() {
    this->name = name;
}

inline std::string GameConfig::getName() const{
    return name;
}

inline PlayerCount GameConfig::getPlayerCount() const{
    return playerCount;
}

inline bool GameConfig::hasAudience() const{
    return audience;
}

inline Setup GameConfig::getSetup() const{
    return setup;
}

void GameConfig::setName(const std::string& name){
    this->name = name;
}

void GameConfig::setPlayerCount(const PlayerCount& playerCount){
    this->playerCount = playerCount;
}

void GameConfig::setAudience(bool audience){
    this->audience = audience;
}

void GameConfig::setSetup(const Setup& setup){
    this->setup = setup;
}





