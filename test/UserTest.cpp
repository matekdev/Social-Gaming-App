#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "User.h"

struct UserTestParams {
    const std::uintptr_t ID = 2020314;
    const std::string NAME = "TestName";
};

class UserTest : public testing::Test {
public:
    User* user;
    UserTestParams params;

    UserTest() {
        user = new User(UserId(params.ID));
    }

    ~UserTest() {
        delete user;
    }
};

TEST_F(UserTest, UserConstructor) {
    EXPECT_EQ(params.ID, user->getUserIdValue());
    EXPECT_EQ("user"+std::to_string(params.ID), user->getUserNameValue());
}

TEST_F(UserTest, setUserName) {
    user->setUserName(UserName(params.NAME));
    EXPECT_EQ(params.NAME, user->getUserNameValue());
}

TEST_F(UserTest, getUserNameValue) {
    EXPECT_EQ("user"+std::to_string(params.ID), user->getUserNameValue());
}

TEST_F(UserTest, getUserId) {
    UserId userId(params.ID);
    EXPECT_EQ(userId, user->getUserId());
}

TEST_F(UserTest, getUserIdValue) {
    EXPECT_EQ(params.ID, user->getUserIdValue());
}

TEST_F(UserTest, UserComparison) {
    User user1(UserId(14032020));
    User user2(UserId(14032020));

    EXPECT_TRUE(user1 == user2);

    User user3(UserId(20200314));

    EXPECT_FALSE(user1 == user3);
}