#include "GameSessionList.h"

GameSessionList::GameSessionList() :
        sessionList({}),
        userToInviteCode({}),
        inviteCodeToUser({}) {}

bool GameSessionList::joinGameSession(std::weak_ptr<User>& userRef, const Invitation& invitation) noexcept{
    auto gameSession = findGameSession(invitation);

    if(gameSession != sessionList.end() && !gameSession->isGameStarted()){
        gameSession->addPlayer(userRef);
        addToUserInviteCodeMaps(userRef.lock()->getUserId(), invitation);
        return true;
    }

    return false;
}

bool GameSessionList::leaveGameSession(std::weak_ptr<User>& userRef, const Invitation& invitation) noexcept{
    auto gameSession = findGameSession(invitation);

    if(gameSession != sessionList.end() && !gameSession->isGameStarted()){
        gameSession->removePlayer(userRef);
        removeFromUserInviteCodeMaps(userRef.lock()->getUserId(), invitation);
        return true;
    }

    return false;
}

Invitation GameSessionList::startGameSession(std::weak_ptr<User>& owner, const std::string& gameFilePath) noexcept{
    GameSession gameSession(owner, gameFilePath);
    auto inviteCode = gameSession.getInvitationCode();
    sessionList.push_back(std::move(gameSession));
    return inviteCode;
}

bool GameSessionList::endGameSession(const Invitation& invitation) noexcept{
    auto session = findGameSession(invitation);
    
    if (session != sessionList.end()){
        sessionList.erase(session);
        return true;
    }

    return false;
}

void GameSessionList::addMessages(const std::list<Message>& messages) noexcept{
    messageBuffer.insert(messageBuffer.end(), messages.begin(), messages.end());
}

std::list<Message> GameSessionList::updateAndGetAllMessages() noexcept{

    for(auto& message : messageBuffer){
        addMessageToCorrectSession(message);
        addMessageToCorrectGame(message);
    }

    messageBuffer.clear();

    std::list<std::pair<UserId, std::string>> messages {};

    for(auto& session : sessionList){
        auto message = session.getAndClearAllMessages();
        messages.insert(messages.end(), message.begin(), message.end());
    }
    
    std::list<Message> result {};
    
    std::transform(messages.begin(), messages.end(), std::back_inserter(result), [](std::pair<UserId, std::string>& message){
        return Message{User(message.first), message.second};
    });

    return result;
}

void GameSessionList::addMessageToCorrectSession(const Message& message) noexcept{
	auto userIterator = userToInviteCode.find(message.user.getUserId());

	if(userIterator != userToInviteCode.end()) {
        auto sessionIterator = findGameSession(userIterator->second);
                            
        if(sessionIterator != sessionList.end()) {
            sessionIterator->addLobbyMessage(message.payload);
        }
    }
}

void GameSessionList::addMessageToCorrectGame(const Message& message) noexcept {
    auto userIterator =  userToInviteCode.find(message.user.getUserId());

    if(userIterator != userToInviteCode.end()) {
        auto sessionIterator = findGameSession(userIterator->second);

        if(sessionIterator != sessionList.end()) {
            sessionIterator->addGameMessage(message.user.getUserId(), message.payload);
        }
    }
}

bool GameSessionList::startGameInGameSession(std::weak_ptr<User>& user, const Invitation& invitation) {
    auto gameSession = findGameSession(invitation);

    //TODO: Check for owner, sessionList.IsOwner currently crashes program.
    if (gameSession != sessionList.end() && !gameSession->isGameStarted()){
        gameSession->startGame();
        return true;
    }

    return false;
}

bool GameSessionList::endGameInGameSession(std::weak_ptr<User>& user, const Invitation& invitation) {
    auto gameSession = findGameSession(invitation);

    //TODO: Check for owner, sessionList.IsOwner currently crashes program.
    if (gameSession != sessionList.end()) {
        gameSession->endGame();
        return true;
    }

    return false;
}

// Finds corresponding game session to provided Invitation code
// Returns the GameSession if one is found or an empty optional if none is found 
std::list<GameSession>::iterator GameSessionList::findGameSession(const Invitation& invitation) noexcept{
    return std::find_if(sessionList.begin(), sessionList.end(), [invitation](const GameSession& session){
        return session.getInvitationCode().toString() == invitation.toString();
    });
}

bool GameSessionList::isUserInSession(const std::weak_ptr<User>& user) const noexcept{
    try{
        auto invite = userToInviteCode.at(user.lock()->getUserId());
        return true;
    }
    catch (std::out_of_range&) {
        return false;
    }
}


void GameSessionList::addToUserInviteCodeMaps(const UserId& id, const Invitation& invite) noexcept{
    userToInviteCode.insert(std::make_pair(id, invite));

    auto userList = inviteCodeToUser.find(invite);

    if(userList == inviteCodeToUser.end()){
        inviteCodeToUser.insert(std::make_pair(invite, std::vector<UserId>({id})));
    }
    else {
        userList->second.push_back(id);
    }
}

void GameSessionList::removeFromUserInviteCodeMaps(const UserId& id, const Invitation& invite) noexcept{
    auto userId = userToInviteCode.find(id);

    userToInviteCode.erase(userId);

    auto userList = inviteCodeToUser.find(invite);

    if(userList->second.size() <= 1){
        inviteCodeToUser.erase(userList);
    }
    else {
        userList->second.erase(std::find(userList->second.begin(), userList->second.end(), id));
    }  

}
    