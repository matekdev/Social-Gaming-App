#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "UserManager.h"

struct UserManagerTestParams {
    const std::uintptr_t ID = 2020314;
    const std::uintptr_t ID2 = 4132020;
};

class UserManagerTest : public testing::Test {
public:
    UserManager* userManager;
    UserManagerTestParams params;

    UserManagerTest() {
        userManager = new UserManager();
    }

    ~UserManagerTest() {
        delete userManager;
    }
};

TEST_F(UserManagerTest, createUser) {
    userManager->createUser(UserId(params.ID));
    EXPECT_EQ(1, userManager->size());

    userManager->createUser(UserId(params.ID));
    EXPECT_EQ(1, userManager->size());

    userManager->createUser(UserId(params.ID2));
    EXPECT_EQ(2, userManager->size());
}

TEST_F(UserManagerTest, destroyUser) {
    EXPECT_FALSE(userManager->destroyUser(UserId(params.ID)));

    userManager->createUser(UserId(params.ID));
    EXPECT_TRUE(userManager->destroyUser(UserId(params.ID)));
}

TEST_F(UserManagerTest, getUserRef) {
    EXPECT_EQ(std::nullopt, userManager->getUserRef(UserId(params.ID)));

    userManager->createUser(UserId(params.ID));
    EXPECT_EQ(params.ID, userManager->getUserRef(UserId(params.ID))->lock()->getUserIdValue());
}

TEST_F(UserManagerTest, iterator) {
    userManager->createUser(UserId(params.ID));

    for (const auto& user : *userManager) {
        EXPECT_EQ(params.ID, user.first.getId());
        EXPECT_EQ(params.ID, user.second->getUserIdValue());
    }
}

TEST_F(UserManagerTest, size) {
    userManager->createUser(UserId(params.ID));
    userManager->createUser(UserId(params.ID2));
    EXPECT_EQ(2, userManager->size());

    userManager->destroyUser(UserId(params.ID2));
    EXPECT_EQ(1, userManager->size());

    userManager->destroyUser(UserId(params.ID));
    EXPECT_EQ(0, userManager->size());
}