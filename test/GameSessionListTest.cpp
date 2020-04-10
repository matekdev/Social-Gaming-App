#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "GameSessionList.h"

struct GameSessionListTestParams {
    const std::string gamePath = "../TestGames/Test.json";
    const uintptr_t id = 1010;
    const Invitation invite = Invitation::createInvitationFromString("TestInvite");
    std::weak_ptr<User> owner = std::make_shared<User>(User(UserId(id)));
    GameSession gameSession = GameSession(owner, gamePath);
};

class GameSessionListTest : public testing::Test {
public:
    GameSessionList* gameSessionList;
    GameSessionListTestParams params;

    GameSessionListTest() {

    }

    ~GameSessionListTest() {
        delete gameSessionList;
    }
};

TEST_F(GameSessionListTest, joinGameSession) {
    gameSessionList->joinGameSession(params.owner, params.invite);
    EXPECT_FALSE(gameSessionList->isUserInSession(params.owner));

    gameSessionList->startGameSession(params.owner, params.gamePath);
    EXPECT_TRUE(gameSessionList->isUserInSession(params.owner));
}

TEST_F(GameSessionListTest, leaveGameSession) {
    gameSessionList->joinGameSession(params.owner, params.invite);
    EXPECT_FALSE(gameSessionList->isUserInSession(params.owner));

    gameSessionList->startGameSession(params.owner, params.gamePath);
    gameSessionList->leaveGameSession(params.owner, params.invite);
    EXPECT_FALSE(gameSessionList->isUserInSession(params.owner));
}

TEST_F(GameSessionListTest, commenceGameSession) {
    gameSessionList->startGameSession(params.owner, params.gamePath);
    EXPECT_TRUE(gameSessionList->isUserInSession(params.owner));
}

TEST_F(GameSessionListTest, startGameInGameSession) {
    EXPECT_FALSE(gameSessionList->startGameInGameSession(params.owner, params.invite));

    gameSessionList->startGameSession(params.owner, params.gamePath);
    EXPECT_TRUE(gameSessionList->startGameInGameSession(params.owner, params.invite));
}

TEST_F(GameSessionListTest, endGameInGameSession) {
    gameSessionList->startGameSession(params.owner, params.gamePath);
    gameSessionList->startGameInGameSession(params.owner, params.invite);

    EXPECT_TRUE(gameSessionList->endGameInGameSession(params.owner, params.invite));
}

TEST_F(GameSessionListTest, addMessages) {
    User user(UserId(params.id));
    gameSessionList->addMessages(std::list<Message>({Message{user, "Test"}}));
    EXPECT_TRUE(!gameSessionList->updateAndGetAllMessages().empty());
}

TEST_F(GameSessionListTest, updateAndGetAllMessages) {
    User user(UserId(params.id));
    gameSessionList->addMessages(std::list<Message>({Message{user, "Test"}}));
    auto messages = gameSessionList->updateAndGetAllMessages();
    EXPECT_TRUE(!messages.empty());
    EXPECT_TRUE(gameSessionList->updateAndGetAllMessages().empty());
}


TEST_F(GameSessionListTest, isUserInSession) {
    EXPECT_FALSE(gameSessionList->isUserInSession(params.owner));

    gameSessionList->startGameSession(params.owner, params.gamePath);
    EXPECT_TRUE(gameSessionList->isUserInSession(params.owner));
}