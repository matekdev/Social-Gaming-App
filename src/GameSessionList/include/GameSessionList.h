#include "GameSession.h"
#include "Invitation.h"
#include "UserManager.h"
#include <unordered_map>
#include <list>
#include <optional>
#include <sstream>
#include <algorithm>

struct Message {
    User user;
    std::string payload;
};

class GameSessionList {
    public:
        GameSessionList();
        
        bool joinGameSession(std::weak_ptr<User>&, const Invitation&) noexcept;
        bool leaveGameSession(std::weak_ptr<User>&, const Invitation&) noexcept;
        
        Invitation startGameSession(std::weak_ptr<User> &owner, const std::string &gameFilePath) noexcept;
        bool endGameSession(const Invitation &invitation) noexcept;

        bool startGameInGameSession(std::weak_ptr<User>& user, const Invitation&);
        bool endGameInGameSession(std::weak_ptr<User>& user, const Invitation& invitation);

        void addMessages(const std::list<Message>&) noexcept;
        std::list<Message> updateAndGetAllMessages() noexcept;

        bool isUserInSession(const std::weak_ptr<User>&) const noexcept;
    
    private:
	    struct InvitationHash {
            std::size_t operator()(const Invitation& invitation) const {
                return std::hash<std::string>()(invitation.toString());
            }
        };

        struct UserIdHash {
            std::size_t operator()(const UserId& id) const {
                //casting uintptr to string
                std::stringstream stream;
                stream << id.getId();
                
                return std::hash<std::string>()(stream.str());
            }
        };

        std::list<GameSession> sessionList;
        std::list<Message> messageBuffer;

        std::unordered_map<UserId, Invitation, UserIdHash> userToInviteCode;
        std::unordered_map<Invitation, std::vector<UserId>, InvitationHash> inviteCodeToUser;


        std::list<GameSession>::iterator findGameSession(const Invitation&) noexcept;
        void addToUserInviteCodeMaps(const UserId&, const Invitation&) noexcept;
        void removeFromUserInviteCodeMaps(const UserId&, const Invitation&) noexcept;
        void addMessageToCorrectSession(const Message&) noexcept;
        void addMessageToCorrectGame(const Message&) noexcept;
};