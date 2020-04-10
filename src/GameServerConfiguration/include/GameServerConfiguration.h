#ifndef GAME_SERVER_CONFIGURATION_H
#define GAME_SERVER_CONFIGURATION_H

#include <string>
#include <sstream> //for HTML File Parsing
#include <nlohmann/json.hpp>
#include <map>
#include <vector>
#include <experimental/filesystem>
#include <unistd.h> //for json verification
#include <fstream> //for json verification
#include <string> //for file paths
#include <iostream> //for error logging

class GameServerConfiguration{
    public:
        // Commands need to be implemented and added in GameServer.cpp
        enum CommandType{
            FIRST,
            
            CREATE_SESSION,
            DISCONNECT,
            HELP,
            JOIN_SESSION,
            LEAVE_SESSION,
            SHUTDOWN,
            START_GAME,
            USERNAME,

            NULL_COMMAND,
            LAST,
        };

        GameServerConfiguration(const std::string&);

        unsigned short getPort() const noexcept;
        std::string getHtmlFileContent() const noexcept;
        std::vector<std::string> getGameFileList() const noexcept;
        CommandType string2Command(const std::string&) const noexcept;
        std::string command2Description(const CommandType&) const noexcept;

        static std::vector<std::string> splitCommand(const std::string& command);
        static std::string getAllCommandDescriptions(const GameServerConfiguration &configuration) noexcept;
        static std::vector<std::string> getCommandArgumentsFromString(const std::string& command) noexcept;
        static GameServerConfiguration::CommandType getCommandTypeFromString(const GameServerConfiguration& configuration, const std::string& command) noexcept;

    private:
        struct Port {
            unsigned short id = 0;
        } port;

        struct Path{
            std::string path = "";
        } htmlFile, gameFolder;

        std::map<std::string, GameServerConfiguration::CommandType> stringToCommandMap;
        std::map<GameServerConfiguration::CommandType, std::string> commandToDescriptionMap;

        void configureServer(const nlohmann::json&) noexcept;
        void verifyJSON(const nlohmann::json&) const noexcept;
        std::optional<GameServerConfiguration::CommandType> string2CommandType(const std::string&) const noexcept;
        void configureCommands(const std::string&, const nlohmann::json&);
};

#endif