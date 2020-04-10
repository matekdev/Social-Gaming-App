#include "GameServerConfiguration.h"

namespace ConfigurationCommandTags{
    const std::string PORT = "Default Port";
    const std::string HTML_PAGE = "HTML Location";
    const std::string GAME_FOLDER = "Game Location";
    const std::string COMMAND_CONFIGURATION = "Commands Configuration";
    const std::string COMMAND_PREFIX_SYMBOL = "Prefix Symbol";
    const std::string COMMAND_LIST = "Command List";
    const std::string COMMAND_TYPE = "Type";
    const std::string COMMAND_DESCRIPTION = "Description";
    const std::string COMMAND_TRIGGER = "User Commands";
};

GameServerConfiguration::GameServerConfiguration(const std::string &configurationFilePath){
	std::ifstream configurationFile(configurationFilePath, std::ifstream::in);
	nlohmann::json configuration = nlohmann::json::parse(configurationFile);
    configureServer(configuration);
}

unsigned short GameServerConfiguration::getPort() const noexcept{
    return GameServerConfiguration::port.id;
}

std::string GameServerConfiguration::getHtmlFileContent() const noexcept{
    std::ifstream htmlFileStream{GameServerConfiguration::htmlFile.path};

    std::string htmlFileContents{std::istreambuf_iterator<char>(htmlFileStream), std::istreambuf_iterator<char>()};
    
    return htmlFileContents;
}

GameServerConfiguration::CommandType GameServerConfiguration::string2Command(const std::string& input) const noexcept{
    try {
        
        return stringToCommandMap.at(input);

    } catch (std::out_of_range& error) {
        
        return GameServerConfiguration::CommandType::NULL_COMMAND;

    }
}

std::string GameServerConfiguration::command2Description(const CommandType& input) const noexcept{
    return commandToDescriptionMap.at(input);
}

void GameServerConfiguration::configureServer(const nlohmann::json &configurationFile) noexcept{
    verifyJSON(configurationFile);
    GameServerConfiguration::port.id = configurationFile
        .at(ConfigurationCommandTags::PORT)
        .get<unsigned short>();

    GameServerConfiguration::htmlFile.path = configurationFile
        .at(ConfigurationCommandTags::HTML_PAGE)
        .get<std::string>();

    GameServerConfiguration::gameFolder.path = configurationFile
        .at(ConfigurationCommandTags::GAME_FOLDER)
        .get<std::string>();

    std::string commandPrefix = configurationFile
        .at(ConfigurationCommandTags::COMMAND_CONFIGURATION)
        .at(ConfigurationCommandTags::COMMAND_PREFIX_SYMBOL)
        .get<std::string>();

    nlohmann::json commandList = configurationFile
        .at(ConfigurationCommandTags::COMMAND_CONFIGURATION)
        .at(ConfigurationCommandTags::COMMAND_LIST);

    configureCommands(commandPrefix, commandList);
}

void GameServerConfiguration::verifyJSON(const nlohmann::json &configurationFile) const noexcept{
    //TODO: add check if field actually exist

    auto port = configurationFile.at(ConfigurationCommandTags::PORT);
	//string to short conversion check
	if(port.get<unsigned short>() != port.get<std::intmax_t>()){
		std::cout << "Port out of range\n";
	    std::exit(-1);
	}

	std::string htmlpath = configurationFile.at(ConfigurationCommandTags::HTML_PAGE);
	//html path check for valid file
	if(access(htmlpath.c_str(), R_OK) == -1){
	    std::cout << "Unable to open HTML index file: " << htmlpath << "\n";
        std::exit(-1);
	}

    //... add more checks
}

std::optional<GameServerConfiguration::CommandType> GameServerConfiguration::string2CommandType(const std::string& commandString) const noexcept{

    if(commandString == "CREATE_SESSION"){
        return GameServerConfiguration::CommandType::CREATE_SESSION;
    }
    else if(commandString == "DISCONNECT"){
        return GameServerConfiguration::CommandType::DISCONNECT;
    } 
    else if(commandString == "HELP"){
        return GameServerConfiguration::CommandType::HELP;        
    }
    else if(commandString == "JOIN_SESSION"){
        return GameServerConfiguration::CommandType::JOIN_SESSION;        
    }
    else if(commandString == "LEAVE_SESSION"){
        return GameServerConfiguration::CommandType::LEAVE_SESSION;
    }
    else if(commandString == "SHUTDOWN"){
        return GameServerConfiguration::CommandType::SHUTDOWN;
    }
    else if(commandString == "START_GAME"){
        return GameServerConfiguration::CommandType::START_GAME;
    }
    else if(commandString == "USERNAME"){
        return GameServerConfiguration::CommandType::USERNAME;
    }
    else{
        return std::nullopt;
    }
}

void GameServerConfiguration::configureCommands(const std::string& commandPrefix, const nlohmann::json& commandList) {
    for(auto& command : commandList){
        std::string commandTypeString = command
            .at(ConfigurationCommandTags::COMMAND_TYPE)
            .get<std::string>();

        std::optional<GameServerConfiguration::CommandType> type = string2CommandType(commandTypeString);
        
        if(type != std::nullopt){
            std::string commandDescription = command
                .at(ConfigurationCommandTags::COMMAND_DESCRIPTION)
                .get<std::string>();

            commandToDescriptionMap.insert(std::make_pair(type.value(), commandDescription));

            std::vector<std::string> userCommands = command
                .at(ConfigurationCommandTags::COMMAND_TRIGGER)
                .get<std::vector<std::string>>();

            for(auto& commandString : userCommands){
                stringToCommandMap.insert(std::make_pair(commandPrefix + commandString, type.value()));
            }
        }
    }

    //Null commands does not and should not have any description
    commandToDescriptionMap.insert(std::make_pair(GameServerConfiguration::CommandType::NULL_COMMAND, ""));

    //placeholder enums for iterating should also not have any description
    commandToDescriptionMap.insert(std::make_pair(GameServerConfiguration::CommandType::FIRST, ""));
    commandToDescriptionMap.insert(std::make_pair(GameServerConfiguration::CommandType::LAST, ""));

}

std::vector<std::string> GameServerConfiguration::splitCommand(const std::string& command){
    std::istringstream iss(command);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    return results;
}

std::string GameServerConfiguration::getAllCommandDescriptions(const GameServerConfiguration &configuration) noexcept{
    std::string str = "";
    
    //is there a better way?
    for(int index = GameServerConfiguration::CommandType::FIRST; index < GameServerConfiguration::CommandType::LAST; index++){
        str += configuration.command2Description(static_cast<GameServerConfiguration::CommandType> (index)) + "\n";
    }

    return str;
}

std::vector<std::string> GameServerConfiguration::getCommandArgumentsFromString(const std::string& command) noexcept{
    //TODO: make this more intelligent
    //all this does right now is split commands based on whitespace and remove the command at the 0th position 
    std::vector<std::string> commandParts = splitCommand(command);
    commandParts.erase(commandParts.begin());
    return commandParts;
}

GameServerConfiguration::CommandType GameServerConfiguration::getCommandTypeFromString(const GameServerConfiguration& configuration, const std::string& command) noexcept{
    std::vector<std::string> commandParts = splitCommand(command);

    std::string commandWithoutParameters = commandParts.at(0);

    return configuration.string2Command(commandWithoutParameters);
}

std::vector<std::string> GameServerConfiguration::getGameFileList() const noexcept{
    std::vector<std::string> result {};
    
    for(auto& file : std::experimental::filesystem::directory_iterator(gameFolder.path)){
        result.push_back(file.path().string());
    }

    return result;
}
