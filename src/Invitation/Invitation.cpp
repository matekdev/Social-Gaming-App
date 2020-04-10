#include "include/Invitation.h"
#include <random>
#include <cmath>

unsigned long Invitation::count = 0;

Invitation::Invitation() : invitationCode( generateInvitationCode() )
    {}

Invitation::Invitation(const std::string& invitationCode) : invitationCode( invitationCode )  
    {}

std::string Invitation::toString() const {
    return invitationCode;
}

// Returns an invitation code based on the count value and a random number (as a string)
// This is in the format: <COUNT_VALUE><SEPERATOR><RANDOM_NUMBER>
std::string Invitation::generateInvitationCode(){
    std::random_device rd;
    std::mt19937 gen(rd());
    long min = pow(10, RANDOM_CODE_LENGTH - 1);
    long max = pow(10, RANDOM_CODE_LENGTH) - 1;
    std::uniform_int_distribution<long> dis(min, max);
    auto countString = std::to_string(++count);
    if (countString.length() < MINIMUM_COUNT_STRING_LENGTH){
        countString = std::string(MINIMUM_COUNT_STRING_LENGTH - countString.length(), '0').append(countString);
    }
    auto randomString = std::to_string(dis(gen));
    auto inviteCode = countString.append(SEPARATOR).append(randomString);
    return inviteCode;
}