#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Invitation.h"

TEST(InvitationTest, toString){
    const std::string invitationString = "001x256";
    Invitation invitation { Invitation::createInvitationFromString(invitationString) };
    EXPECT_EQ(invitation.toString(), invitationString);
}

TEST(InvitationTest, noRandomDuplicates){
    Invitation invitation1 { Invitation::createNewInvitation() };
    Invitation invitation2 { Invitation::createNewInvitation() };
    ASSERT_NE(&invitation1, &invitation2);
    EXPECT_NE(invitation1, invitation2);
}

TEST(InvitationTest, noStringDuplicates){
    const std::string invitationString = "001x256";
    Invitation invitation1 { Invitation::createInvitationFromString(invitationString) };
    Invitation invitation2 { Invitation::createInvitationFromString(invitationString) };
    ASSERT_NE(&invitation1, &invitation2);
    EXPECT_EQ(invitation1, invitation2);
}