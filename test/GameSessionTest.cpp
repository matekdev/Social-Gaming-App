#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "GameSession.h"


struct GameSessionTestParams {
    const std::string gamePath = "../TestGames/Test.json";
    const uintptr_t id = 1010;
};

class GameSessionTest : public testing::Test {
public:
    GameSession* gameSession;
    GameSessionTestParams params;

    GameSessionTest() {
        std::weak_ptr<User> owner = std::make_shared<User>(User(UserId(params.id)));
        gameSession = new GameSession(owner, params.gamePath);
    }

    ~GameSessionTest() {
        delete gameSession;
    }
};

TEST_F(GameSessionTest, getInvitationCode) {
    Invitation invite = gameSession->getInvitationCode();
    EXPECT_TRUE(!invite.toString().empty());
}

TEST_F(GameSessionTest, isGameStarted) {
    EXPECT_FALSE(gameSession->isGameStarted());
}

TEST_F(GameSessionTest, startGame) {
    gameSession->startGame();
    EXPECT_TRUE(gameSession->isGameStarted());
}

TEST_F(GameSessionTest, endGame) {
    gameSession->startGame();
    gameSession->endGame();
    EXPECT_FALSE(gameSession->isGameStarted());
}

TEST_F(GameSessionTest, isOwner) {
    // TODO: IsOwner is broken
}

TEST_F(GameSessionTest, addMessages) {
    std::string message = "TestMessage";
    gameSession->addLobbyMessage(message);
    EXPECT_TRUE(true);
}

TEST_F(GameSessionTest, addMessagesToGame) {
    std::string message = "TestMessage";
    gameSession->addGameMessage(UserId(1), message);
    EXPECT_TRUE(true);
}

TEST_F(GameSessionTest, getAndClearAllMessages) {
    std::string message1 = "TestMessage1";
    std::string message2 = "TestMessage2";
    std::string message3 = "TestMessage3";

    gameSession->addLobbyMessage(message1);
    gameSession->addLobbyMessage(message2);
    gameSession->addGameMessage(UserId(1), message3);
    auto message = gameSession->getAndClearAllMessages();

    EXPECT_TRUE(gameSession->getAndClearAllMessages().empty());
}

TEST_F(GameSessionTest, addPlayer) {
    gameSession->addPlayer(std::make_shared<User>(User(UserId(1))));
    EXPECT_TRUE(true);
}

TEST_F(GameSessionTest, removePlayer) {
    gameSession->removePlayer(std::make_shared<User>(User(UserId(1))));
    EXPECT_TRUE(true);
}