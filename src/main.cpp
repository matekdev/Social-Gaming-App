#include "GameServer.h"
#include "GameServerConfiguration.h"

#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

static std::vector<std::unique_ptr<GameServer>> servers;

static std::atomic<bool> exit_thread_flag{false};
static const std::string SERVER_CONFIGURATION_FILE_LOCATION = "config/ServerProperties.json";
static GlobalMessage globalMessage;

static void setupServers(int, char* []);
static std::string getConfigurationFilePath(int, char* []);
static void startServer(GameServer& server);

static void OnConnect(networking::Connection);
static void OnDisconnect(networking::Connection);

int main(int argc, char* argv[]){

	setupServers(argc, argv);

	std::vector<std::thread> serverThreads;

	for(auto& gameServer : servers){
		std::thread serverThread(startServer, std::ref(*gameServer));
		serverThreads.push_back(std::move(serverThread));
	}

	std::string message;

	do{
		globalMessage.message = message;
		std::getline(std::cin, message);
	} while (message != "shutdown");

	exit_thread_flag = true;

	for(auto& serverThread : serverThreads){
		serverThread.join();
	}

	return 0;
}

static void setupServers(int argumentCount, char* argumentVector[]){
	std::string configurationFilePath = getConfigurationFilePath(argumentCount, argumentVector);
	std::unique_ptr<GameServerConfiguration> serverConfiguration(new GameServerConfiguration(configurationFilePath));
	
	std::unique_ptr<UserManager> users(new UserManager());

	unsigned short port = serverConfiguration->getPort();
	std::string htmlContents = serverConfiguration->getHtmlFileContent();
	std::unique_ptr<networking::Server> server (new networking::Server(port, htmlContents, OnConnect, OnDisconnect));
	
	std::unique_ptr<GameServer> mainServer(new GameServer(server, serverConfiguration, users));

	servers.push_back(std::move(mainServer));

	std::cout << "Setup Server on Port: " << port << '\n';

	//if there are multiple servers, repeat
}

static std::string getConfigurationFilePath(int argumentCount, char* argumentVector[]){
	if(argumentCount <= 1){
		return SERVER_CONFIGURATION_FILE_LOCATION;
		//TODO: if config file does not exist, create a default one
	}
	else{
		return argumentVector[1];
	}
}

static void startServer(GameServer& server){
	while (exit_thread_flag == false) {

		//if there are multiple servers, maybe also need to check which server this messages should go to
		server.setGlobalMessage(globalMessage);

		server.tick();
		sleep(1);
	}
}

//teacher provided functions
static void OnConnect(networking::Connection c) {
	std::cout << "New connection found: " << c.id << "\n";

	//if multiple servers, need to find correct server first
	servers.at(0)->addUser(c);
}

//teacher provided functions
static void OnDisconnect(networking::Connection c) {
	std::cout << "Connection lost: " << c.id << "\n";
		
	//if multiple servers, need to find correct server first
	servers.at(0)->removeUser(c);
}