
#ifndef GAME_PARSER_H
#define GAME_PARSER_H
 
#include <list>
#include <nlohmann/json.hpp>

#include <cassert>
#include <fstream>
#include <string>
#include <vector>

#include "Game.h"

class GameParser{
public:
//The constructor should take in a string which is the filepath of the appropriate Game JSON, create the game, and store the game in a member variable
    GameParser(const std::string&);
    //This returns the Game object that was created by the Constructor
    //Unique ptr gets returned and reset so this object should be deleted after because it should be useless
    std::unique_ptr<Game> getGame() noexcept;

    GameState createGameState(nlohmann::json gameJson);


private:
    nlohmann::json fileToJson(const std::string& pathName);

    std::unique_ptr<Game> game;
    bool hasGame;
    const std::string CONFIGURATION = "configuration";
    const std::string PLAYER_COUNT = "player count";
    const std::string MIN = "min";
    const std::string MAX = "max";
    const std::string ROUNDS = "Rounds";
    const std::string NAME = "name";
    const std::string AUDIENCE = "audience";
    const std::string SETUP = "setup";
    const std::string VARIABLES = "variables";
    const std::string PER_PLAYER = "per-player";
    const std::string PER_AUDIENCE = "per-audience";
    const std::string CONSTANTS = "constants";

    void insertGameStateValue(GameState &gameState, const nlohmann::json& key, const nlohmann::json& value,
                              const GameState::ValueType& valueType) const;

    void populateValueList(StateValueList &list, const nlohmann::json &jsonValue) const;

    void populateValueMap(StateValueMap &map, const nlohmann::json &jsonValue) const;
};
#endif
