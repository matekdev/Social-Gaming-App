#include "GameServer.h"

GameServer::GameServer(std::unique_ptr<networking::Server>& server, std::unique_ptr<GameServerConfiguration> &configuration, std::unique_ptr<UserManager> &users) :
	server(std::move(server)),
	serverConfiguration(std::move(configuration)),
	users(std::move(users)),
	sessionList(GameSessionList()) 
	{}	

void GameServer::tick(){
	try {
		server->update();
	} 
	catch (std::exception& e) {
		std::cerr << "Exception from Server update:\n"
					<< " " << e.what() << "\n\n";
		return;
	}

	auto incoming = server->receive();

	auto outgoing = gameServerUpdate(*server, incoming);

	server->send(outgoing);
}

void GameServer::setGlobalMessage(const GlobalMessage& message) noexcept{
	globalMessage = message;
}

void GameServer::addUser(const networking::Connection& connection) noexcept{
	users->createUser(UserId(connection.id));
}

void GameServer::removeUser(const networking::Connection& connection) noexcept{
	users->destroyUser(UserId(connection.id));
}

std::deque<networking::Message> GameServer::processMessages(networking::Server& server, const std::deque<networking::Message>& incoming) {
	std::deque<networking::Message> commandResult;
	
	for (networking::Message message : incoming) {

		UserId id(message.connection.id);
        std::optional<std::weak_ptr<User>> user = users->getUserRef(id);
        if (!user.has_value()) continue; // Unlikely to fail, but avoid processing commands from invalid connections.

        auto commandType = GameServerConfiguration::getCommandTypeFromString(*serverConfiguration, message.text);
        std::vector<std::string> commandParams = GameServerConfiguration::getCommandArgumentsFromString(message.text);

        switch (commandType) {
            case GameServerConfiguration::CREATE_SESSION:
                message.text.append(commandCREATE_SESSION(commandParams, user.value()));
                break;
            case GameServerConfiguration::HELP:
                message.text.append(commandHELP());
                break;
            case GameServerConfiguration::JOIN_SESSION:
                message.text.append(commandJOIN_SESSION(commandParams, user.value()));
                break;
            case GameServerConfiguration::LEAVE_SESSION:
                message.text.append(commandLEAVE_SESSION(commandParams, user.value()));
                break;
            case GameServerConfiguration::SHUTDOWN:
                message.text.append(commandSHUTDOWN(commandParams, user.value()));
                break;
            case GameServerConfiguration::START_GAME:
                message.text.append(commandSTART_GAME(commandParams, user.value()));
                break;
            case GameServerConfiguration::USERNAME:
                message.text.append(commandUSERNAME(commandParams, user.value()));
                break;
            case GameServerConfiguration::NULL_COMMAND:
                commandNULL_COMMAND(user.value(), message.text);
                break;
            case GameServerConfiguration::DISCONNECT:
                server.disconnect(message.connection);
                break;
            default:
                std::cout << "Unable to process command, something went horribly wrong.\n";
                break;
        }

        if (commandReturnsResponse(commandType)) {
            commandResult.push_back(networking::Message{message.connection, formatMessageWithUserName(message.text, user->lock()->getUserNameValue())});
        }
	}
	
	return commandResult;
}

std::deque<networking::Message> GameServer::getGlobalMessages(){
	std::deque<networking::Message> result {};
	
	if(!globalMessage.message.empty()){

		for(auto& entry : *users){
			User user = *(entry.second);
			result.push_back({networking::Connection{user.getUserIdValue()}, globalMessage.message});
		}

		globalMessage.message = "";
	}

	return result;
}

std::deque<networking::Message> GameServer::gameServerUpdate(networking::Server& server, const std::deque<networking::Message>& incoming) {

	std::deque<networking::Message> allMessages = {};

	std::deque<networking::Message> globalMessages = getGlobalMessages();
	allMessages.insert(allMessages.end(), globalMessages.begin(), globalMessages.end());

	//doesn't really make sense for command messages to be broadcasted to everyone, so only the person creating the command needs to see the server reply
	std::deque<networking::Message> commandMessages = processMessages(server, incoming);
	allMessages.insert(allMessages.end(), commandMessages.begin(), commandMessages.end());

	//TODO: update all games based on game logic (probs in gamesessionmanager)

	std::list<Message> rawSessionMessages = sessionList.updateAndGetAllMessages();
	std::deque<networking::Message> transformedSessionMessages {};

	std::transform(rawSessionMessages.begin(), rawSessionMessages.end(), std::back_inserter(transformedSessionMessages), [](const Message& message){
		return networking::Message{networking::Connection{message.user.getUserId().getId()}, message.payload};
	});

	allMessages.insert(allMessages.end(), transformedSessionMessages.begin(), transformedSessionMessages.end());

	return allMessages;
}

//region Commands
#pragma region Commands
bool GameServer::commandReturnsResponse(const GameServerConfiguration::CommandType& command) {
    switch(command) {
        case GameServerConfiguration::CREATE_SESSION:
        case GameServerConfiguration::HELP:
        case GameServerConfiguration::JOIN_SESSION:
        case GameServerConfiguration::LEAVE_SESSION:
        case GameServerConfiguration::SHUTDOWN:
        case GameServerConfiguration::START_GAME:
        case GameServerConfiguration::USERNAME:
            return true;
        case GameServerConfiguration::FIRST:
        case GameServerConfiguration::LAST:
        case GameServerConfiguration::NULL_COMMAND:
        case GameServerConfiguration::DISCONNECT:
        default:
            return false;
    }
}

std::string GameServer::commandCREATE_SESSION(const std::vector<std::string>& commandParams, std::weak_ptr<User>& user) {

    if(commandParams.empty()){
        return "\n Unable to create GameSession, please enter a game\n";
    }

    std::string gameName = commandParams.at(0);
    std::vector<std::string> gameFileList = serverConfiguration->getGameFileList();

    auto gameFileIterator = std::find_if(gameFileList.begin(), gameFileList.end(), [gameName](const std::string& fileName){
        if (gameName.size() > fileName.size()) {
            return false;
        }
        else {   
            return std::equal(gameName.rbegin(), gameName.rend(), fileName.rbegin());
        }
    });

    if(gameFileIterator == gameFileList.end()){
        return "\n Unable to create GameSession: Game " + gameName + " does not exist";
    }
    
    auto createdSessionInvitation = sessionList.startGameSession(user, *gameFileIterator);

    std::string response;
    response.append("\n Created Lobby with Invitation code: ");
    response.append(createdSessionInvitation.toString());
    return response;
}

std::string GameServer::commandHELP() {
    std::string response;
    response.append("\n List of user commands: \n");
    response.append(GameServerConfiguration::getAllCommandDescriptions(*serverConfiguration));
    return response;
}

std::string GameServer::commandJOIN_SESSION(const std::vector<std::string>& commandParams, std::weak_ptr<User>& user) {
    std::string response;

    if (commandParams.empty()) {
        response.append("\n Unable to join Session. Parameter formatting is incorrect.");
        return response;
    }

    Invitation userProvidedCode = Invitation::createInvitationFromString(commandParams.at(0));
    sessionList.joinGameSession(user, userProvidedCode) ? response.append("\n Joining Session.") : response.append("\n Unable to join Session. Does the Session exist?");

    return response;
}

std::string GameServer::commandLEAVE_SESSION(const std::vector<std::string>& commandParams, std::weak_ptr<User>& user) {
    std::string response;

    if (commandParams.empty()) {
        response.append("\n Unable to leave Session. Parameter formatting is incorrect.");
        return response;
    }

    Invitation userProvidedCode = Invitation::createInvitationFromString(commandParams.at(0));
    sessionList.leaveGameSession(user, userProvidedCode) ? response.append("\n Leaving Session.") : response.append("\n Unable to leave Session. Has the Game already started?");

    return response;
}

std::string GameServer::commandSHUTDOWN(const std::vector<std::string>& commandParams, std::weak_ptr<User>& user) {
    std::string response;

    if (commandParams.empty()) {
        response.append("\n Unable to shutdown Game. Parameter formatting is incorrect.");
        return response;
    }

    Invitation userProvidedCode = Invitation::createInvitationFromString(commandParams.at(0));
    sessionList.endGameInGameSession(user, userProvidedCode) ? response.append("\n Ending Game.") : response.append("\n Unable to end Game. Are you the owner?");

    return response;
}

std::string GameServer::commandSTART_GAME(const std::vector<std::string> &commandParams, std::weak_ptr<User> &user) {
    std::string response;

    if (commandParams.empty()) {
        response.append("\n Unable to start Game. Parameter formatting is incorrect.");
        return response;
    }

    Invitation userProvidedCode = Invitation::createInvitationFromString(commandParams.at(0));
    sessionList.startGameInGameSession(user, userProvidedCode) ? response.append("\n Starting Game.") : response.append("\n Unable to start Game. Are you the owner?");

    return response;
}

std::string GameServer::commandUSERNAME(const std::vector<std::string> &commandParams, std::weak_ptr<User> &user) {
    std::string response;

    if (commandParams.empty()) {
        response.append("\n Unable to change name. Parameter formatting is incorrect.");
        return response;
    }

    auto name = commandParams.at(0);
    user.lock()->setUserName(UserName(name));
    response.append("\n Changing name to: " + name);
    return response;
}

void GameServer::commandNULL_COMMAND(std::weak_ptr<User> &user, const std::string& text) {
    if (sessionList.isUserInSession(user)) {
        sessionList.addMessages(std::list<Message>{Message{*(user.lock()), formatMessageWithUserName(text, user.lock()->getUserNameValue())}});
    } else {
        std::cout << "Unrecognized command from User not in session. \n";
        serverConfiguration->getGameFileList();
    }
}

std::string GameServer::formatMessageWithUserName(const std::string &text, const std::string &username){
    std::string formattedMessage = text;
    formattedMessage.insert(0, username + ": ");
    return formattedMessage;
}

#pragma endregion Commands
//endregion