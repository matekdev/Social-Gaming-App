#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "GameState.h"

struct GameStateTestParams {
    const uintptr_t id1 = 1010;
    const uintptr_t id2 = 1011;
    const uintptr_t id3 = 1012;
    std::list<std::weak_ptr<User>> users = {std::make_shared<User>(User(UserId(id1))),
                                            std::make_shared<User>(User(UserId(id2))),
                                            std::make_shared<User>(User(UserId(id3)))};

    const std::string key = "Key";
    const std::string key2 = "Key2";
    const StateValueBoolean stateValueBool = StateValueBoolean(true);
    const StateValueNumber stateValueNumber = StateValueNumber(1);
    const StateValueString stateValueString = StateValueString("Test");
};

class GameStateTest : public testing::Test {
public:
    GameState* gameState;
    GameStateTestParams params;

    GameStateTest() {
        gameState = new GameState();
    }

    ~GameStateTest() {
        delete gameState;
    }
};

TEST_F(GameStateTest, isGameStarted) {
    EXPECT_FALSE(gameState->isGameStarted());
    gameState->startGame(params.users, params.users);
    EXPECT_TRUE(gameState->isGameStarted());
}

TEST_F(GameStateTest, startGame) {
    gameState->startGame(params.users, params.users);
    SUCCEED();
}

TEST_F(GameStateTest, endGame) {
    EXPECT_FALSE(gameState->isGameStarted());
    gameState->startGame(params.users, params.users);
    EXPECT_TRUE(gameState->isGameStarted());
    gameState->endGame();
    EXPECT_FALSE(gameState->isGameStarted());
}

TEST_F(GameStateTest, getConstant) {
    auto value = gameState->getConstant(params.key);
    EXPECT_FALSE(value.has_value());

    gameState->addValue(params.key, params.stateValueBool, GameState::ValueType::CONSTANT);
    value = gameState->getConstant(params.key);
    EXPECT_TRUE(value.has_value());
}

TEST_F(GameStateTest, getConstantOrVariable) {
    auto value = gameState->getConstantOrVariable(params.key);
    EXPECT_FALSE(value.has_value());

    gameState->addValue(params.key, params.stateValueBool, GameState::ValueType::CONSTANT);
    value = gameState->getConstantOrVariable(params.key);
    EXPECT_TRUE(value.has_value());
}

TEST_F(GameStateTest, getVariable) {
    auto value = gameState->getConstantOrVariable(params.key);
    EXPECT_FALSE(value.has_value());

    gameState->addValue(params.key, params.stateValueBool, GameState::ValueType::VARIABLE);
    value = gameState->getVariable(params.key);
    EXPECT_TRUE(value.has_value());
}

// TODO: Test needs to be updated with proper list of users to pass.
TEST_F(GameStateTest, getPerPlayerValue) {

    auto value = gameState->getPerPlayerValue(params.key);
    EXPECT_FALSE(value.has_value());

    gameState->addValue(params.key, params.stateValueBool, GameState::ValueType::PER_PLAYER);
    value = gameState->getPerAudienceValue(params.key);
    EXPECT_TRUE(value.has_value());

    EXPECT_EQ(value.value().get().front().value.get()->getValueType(), StateValue::ValueType::BOOLEAN);
}

// TODO: Test needs to be updated with proper list of users to pass.
TEST_F(GameStateTest, getPerAudienceValue) {
    auto value = gameState->getPerAudienceValue(params.key);
    EXPECT_FALSE(value.has_value());

    gameState->addValue(params.key, params.stateValueBool, GameState::ValueType::PER_AUDIENCE);
    value = gameState->getPerAudienceValue(params.key);
    EXPECT_TRUE(value.has_value());

    EXPECT_EQ(value.value().get().front().value.get()->getValueType(), StateValue::ValueType::BOOLEAN);
}

TEST_F(GameStateTest, addValue) {
    gameState->addValue("Key1", StateValueBoolean(true), GameState::ValueType::VARIABLE);
    gameState->addValue("Key2", StateValueNumber(1), GameState::ValueType::VARIABLE);
    gameState->addValue("Key3", StateValueString("Test"), GameState::ValueType::VARIABLE);
    EXPECT_TRUE(true);
}

TEST_F(GameStateTest, addMessage) {
    gameState->addMessage(UserId(params.id1), "Test");
    auto messages = gameState->updateAndGetAllMessages();
    EXPECT_TRUE(!messages.empty());
    EXPECT_EQ(messages.front().first, UserId(params.id1));
}

// TODO: Test needs to be updated with proper list of users to pass.
TEST_F(GameStateTest, addMessageToAllPlayers) {
//    gameState->startGame(params.users, params.users);
//    gameState->addMessageToAllPlayers("Test");
//    auto messages = gameState->updateAndGetAllMessages();
//    EXPECT_TRUE(!messages.empty());
//    EXPECT_EQ(messages.front().first, UserId(params.id1));
}

// TODO: Test needs to be updated with proper list of users to pass.
TEST_F(GameStateTest, addMessageToAllAudience) {
//    gameState->startGame(params.users, params.users);
//    gameState->addMessageToAllAudience("Test");
//    auto messages = gameState->updateAndGetAllMessages();
//    EXPECT_TRUE(!messages.empty());
//    EXPECT_EQ(messages.front().first, UserId(params.id1));
}

// TODO: Test needs to be updated with proper list of users to pass.
TEST_F(GameStateTest, addMessageToEntireSession) {

}

TEST_F(GameStateTest, clearMessages) {
    gameState->addMessage(UserId(params.id1), "Test");
    gameState->clearMessages();
    auto messages = gameState->updateAndGetAllMessages();
    EXPECT_TRUE(messages.empty());
}

TEST_F(GameStateTest, updateAndGetAllMessages) {
    gameState->addMessage(UserId(params.id1), "Test1");
    gameState->addMessage(UserId(params.id2), "Test2");
    gameState->addMessage(UserId(params.id3), "Test3");

    auto messages = gameState->updateAndGetAllMessages();
    EXPECT_TRUE(!messages.empty());

    EXPECT_EQ(messages.front().first.getId(), params.id1);
    EXPECT_EQ(messages.front().second, "Test1");

    EXPECT_EQ(messages.back().first.getId(), params.id3);
    EXPECT_EQ(messages.back().second, "Test3");
}