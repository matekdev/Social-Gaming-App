#include "GameSession.h"

GameSession::GameSession(std::weak_ptr<User>& owner, const std::string& gameFilePath) :
        lobbyMessages({}),
        invitationCode (Invitation::createNewInvitation()),
        owner (owner)
{
    auto parser = GameParser(gameFilePath);
    game = parser.getGame();
}

void GameSession::setGame(const std::string& gameFilePath){
    auto parser = GameParser(gameFilePath);
    game = parser.getGame();
}

Invitation GameSession::getInvitationCode() const {
    return invitationCode;
}

//TODO: Check for owner, sessionList.IsOwner currently crashes program.
bool GameSession::isOwner(const UserId& user) const {
    return (user == owner.lock()->getUserId());
}

bool GameSession::isGameStarted() const {
    return game->isStarted();
}

void GameSession::startGame() {
    game->startGame(playerList.users, audienceList.users);
}

void GameSession::endGame() {
    game->endGame();
}

void GameSession::addLobbyMessage(const std::string& message) noexcept{
    lobbyMessages.push_back(message);
}

void GameSession::addGameMessage(const UserId& userID, const std::string& message) noexcept{
    if (isGameStarted()) {
        game->addMessage(userID, message);
    }
}

std::list<std::pair<UserId, std::string>> GameSession::getLobbyAndGameMessages() noexcept{
    std::list<std::pair<UserId, std::string>> result;

    if (game->isStarted()) {
        game->gameTick();

        auto gameMessages = game->updateAndGetAllMessages();
        result.insert(result.end(), gameMessages.begin(), gameMessages.end());
    }

    for(auto& message : lobbyMessages){
        for(auto& player : playerList.users){
           result.emplace_back(player.lock()->getUserId(), message);
        }

        for(auto& audience : audienceList.users){
            result.emplace_back(audience.lock()->getUserId(), message);
        }
    }    

    return result;
}

void GameSession::clearMessages() noexcept {
    lobbyMessages = {};
}

std::list<std::pair<UserId, std::string>> GameSession::getAndClearAllMessages() noexcept{

    auto messages = getLobbyAndGameMessages();

    clearMessages();

    return messages;
}

void GameSession::addPlayer(const std::weak_ptr<User>& player) noexcept{
    playerList.users.push_back(player);
    if (auto playerPtr = player.lock()){
        addLobbyMessage(playerPtr->getUserNameValue() + " has joined the game session as a player");
    }
}

void GameSession::removePlayer(const std::weak_ptr<User>& player) noexcept{
    auto playerIterator = std::find_if(playerList.users.begin(), playerList.users.end(), [player](std::weak_ptr<User>& listMember){
        return player.lock()->getUserId() == listMember.lock()->getUserId();
    });
    
    if(playerIterator != playerList.users.end()){
        playerList.users.erase(playerIterator);
        if (auto playerPtr = player.lock()){
            addLobbyMessage(playerPtr->getUserNameValue() + " has left the game session");
        }
    }
}

void GameSession::addAudienceMember(const std::weak_ptr<User>& audienceMember) noexcept {
    audienceList.users.push_back(audienceMember);
    if (auto audiencePtr = audienceMember.lock()){
        addLobbyMessage(audiencePtr->getUserNameValue() + " has joined the game session as an audience member");
    }
}

void GameSession::removeAudienceMember(const std::weak_ptr<User> &audienceMember) noexcept {
    auto audienceIterator = std::find_if(audienceList.users.begin(), audienceList.users.end(), [audienceMember](std::weak_ptr<User>& listMember){
        return audienceMember.lock()->getUserId() == listMember.lock()->getUserId();
    });

    if(audienceIterator != audienceList.users.end()){
        audienceList.users.erase(audienceIterator);
        if (auto audiencePtr = audienceMember.lock()){
            addLobbyMessage(audiencePtr->getUserNameValue() + " has left the game session");
        }
    }
}

void GameSession::addGameMessageToPlayers(const std::string &message) noexcept {
    if (isGameStarted()) {
        game->addMessageToAllPlayers(message);
    }
}

void GameSession::addGameMessageToAudience(const std::string &message) noexcept {
    if (isGameStarted()) {
        game->addMessageToAllAudience(message);
    }
}

void GameSession::addGameMessageToAllSession(const std::string &message) noexcept {
    if (isGameStarted()) {
        game->addMessageToAllSession(message);
    }
}
