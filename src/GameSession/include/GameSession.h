#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include <iostream> 
#include <string>
#include <list>
#include "UserManager.h"
#include "Invitation.h"
#include "GameParser.h"
#include <list>

class GameSession {
    struct UserList{
        std::list<std::weak_ptr<User>> users;
    };

    public:
        explicit GameSession(std::weak_ptr<User>&, const std::string&);

        bool operator==(const GameSession& gameSession ) const {
            return invitationCode == gameSession.invitationCode;
        }

        void setGame(const std::string&);
        [[nodiscard]] Invitation getInvitationCode() const;
        [[nodiscard]] bool isGameStarted() const;
        void startGame();
        void endGame();

        [[nodiscard]] bool isOwner(const UserId& user) const;

        void addLobbyMessage(const std::string &message) noexcept;
        void addGameMessage(const UserId& userID, const std::string& message) noexcept;
        void addGameMessageToPlayers(const std::string& message) noexcept;
        void addGameMessageToAudience(const std::string& message) noexcept;
        void addGameMessageToAllSession(const std::string& message) noexcept;

        std::list<std::pair<UserId, std::string>> getAndClearAllMessages() noexcept;

        void addPlayer(const std::weak_ptr<User>& player) noexcept;
        void removePlayer(const std::weak_ptr<User>& player) noexcept;

        void addAudienceMember(const std::weak_ptr<User>& audienceMember) noexcept;
        void removeAudienceMember(const std::weak_ptr<User>& audienceMember) noexcept;
        
    private:
        std::list<std::string> lobbyMessages;
        Invitation invitationCode;
        std::unique_ptr<Game> game;
        std::weak_ptr<User>& owner;
        GameSession::UserList playerList;
        GameSession::UserList audienceList;

        void clearMessages() noexcept;
        std::list<std::pair<UserId, std::string>> getLobbyAndGameMessages() noexcept;
};

#endif