#include "GameParser.h"
#include "GameConfig.h"
#include <vector>

namespace Tags{


}
////public methods
GameParser::GameParser(const std::string& path) :
    hasGame(true) 
    {
    nlohmann::json gameJson = fileToJson(path);
    GameState gameState = createGameState(gameJson);
    GameSpecification::GameSpec gameSpec(gameJson);

    game = std::make_unique<Game> (gameSpec, gameState);
}

std::unique_ptr<Game> GameParser::getGame() noexcept{
    assert(hasGame);
    hasGame = false;
    return std::move(game);
}

nlohmann::json GameParser::fileToJson(const std::string& pathName) {
    std::ifstream jsonStream(pathName);
    nlohmann::json jsonConfig = nlohmann::json::parse(jsonStream);    
    return jsonConfig;
}

GameState  GameParser::createGameState(nlohmann::json gameJson) {

    GameState gameState;
    auto gameConfig = GameConfig();
    auto config = gameJson.at(CONFIGURATION);

    for(const auto& item : config.items()){
        if(item.key() == NAME){
            gameConfig.setName(item.value());
        }
        else if (item.key() == AUDIENCE) {
            gameConfig.setAudience(item.value());
        }
        else if (item.key() == PLAYER_COUNT){
            auto playerCount = Configuration::PlayerCount{item.value().at(MAX), item.value().at(MIN)};
            gameConfig.setPlayerCount(playerCount);
        }
        else if(item.key() == SETUP){
            Setup setup = item.value();
            gameConfig.setSetup(setup);
        }
    }

    auto constants = gameJson.at(CONSTANTS);

    for(const auto& constant : constants.items()){
        auto constantType = GameState::ValueType::CONSTANT;
        insertGameStateValue(gameState, constant.key(), constant.value(), constantType);
    }


    auto variable = gameJson.at(VARIABLES);
    for(const auto& value: variable.items()){
        GameState::ValueType variableType = GameState::ValueType::VARIABLE;
        auto s = value.value();
        insertGameStateValue(gameState, value.key(), value.value(), variableType);
    }

    auto perPlayer = gameJson.at(PER_PLAYER);
    for(const auto& pPlayer : perPlayer.items()){
        insertGameStateValue(gameState, pPlayer.key(), pPlayer.value(), GameState::ValueType::PER_PLAYER);
    }

    auto perAudience = gameJson.at(PER_AUDIENCE);
    for(const auto& pAudience : perPlayer.items()){
        insertGameStateValue(gameState, pAudience.key(), pAudience.value(), GameState::ValueType::PER_AUDIENCE);
    }

    return gameState;

}

void GameParser::insertGameStateValue(GameState& gameState, const nlohmann::json& key, const nlohmann::json& value,
                                 const GameState::ValueType& valueType) const {
    if (value.is_number_integer()) {
        StateValueNumber val(value.get<int>());
        gameState.addValue(key, val, valueType);
    } else if(value.is_number_float()) {
        StateValueNumber val(value.get<double>());
        gameState.addValue(key, val, valueType);
    } else if(value.is_string()) {
        StateValueString val(value.get<std::string>());
        gameState.addValue(key, val, valueType);
    } else if(value.is_boolean()) {
        StateValueBoolean val(value.get<bool>());
        gameState.addValue(key, val, valueType);
    } else if (value.is_array()){
        StateValueList val;
        populateValueList(val, value);
        gameState.addValue(key, val, valueType);

    } else if (value.is_object()){
        StateValueMap val;
        populateValueMap(val, value);
        gameState.addValue(key, val, valueType);
    }
}

void GameParser::populateValueList(StateValueList &list, const nlohmann::json& jsonValue) const {
    for (const auto& i : jsonValue.items()){
        const auto& value = i.value();
        if (value.is_number_integer()) {
            StateValueNumber val(value.get<int>());
            list.addValue(val);
        } else if(value.is_number_float()) {
            StateValueNumber val(value.get<double>());
            list.addValue(val);
        } else if(value.is_string()) {
            StateValueString val(value.get<std::string>());
            list.addValue(val);
        } else if(value.is_boolean()) {
            StateValueBoolean val(value.get<bool>());
            list.addValue(val);
        } else if (value.is_array()){
            StateValueList val;
            populateValueList(val, value);
            list.addValue(val);
        } else if (value.is_object()){
            StateValueMap val;
            populateValueMap(val, value);
            list.addValue(val);
        }
    }
}

void GameParser::populateValueMap(StateValueMap &map, const nlohmann::json& jsonValue) const {
    for (const auto& i : jsonValue.items()){
        const auto &key = i.key();
        const auto& value = i.value();
        if (value.is_number_integer()) {
            StateValueNumber val(value.get<int>());
            map.addValue(key, val);
        } else if(value.is_number_float()) {
            StateValueNumber val(value.get<double>());
            map.addValue(key, val);
        } else if(value.is_string()) {
            StateValueString val(value.get<std::string>());
            map.addValue(key, val);
        } else if(value.is_boolean()) {
            StateValueBoolean val(value.get<bool>());
            map.addValue(key, val);
        } else if (value.is_array()){
            StateValueList val;
            populateValueList(val, value);
            map.addValue(key, val);
        } else if (value.is_object()){
            StateValueMap val;
            populateValueMap(val, value);
            map.addValue(key, val);
        }
    }
}