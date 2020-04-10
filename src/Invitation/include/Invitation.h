#ifndef INVITATION_H
#define INVITATION_H

#include <string>

class Invitation {
public:
    [[nodiscard]] std::string toString() const;
    
    // This constructor creates a new Invitation object with a semi-random code.
    // It is intended to be used when a new GameSession is created.
    static Invitation createNewInvitation(){
        return Invitation();
    };

    // This constructor turns wraps string input from the user into an Invitation object, 
    // so that  it may be compared with other Invitation objects.
    static Invitation createInvitationFromString(const std::string& invitationCode){
        return Invitation(invitationCode);
    }

    bool operator==(const Invitation& invitation ) const {
        return invitationCode == invitation.toString();
    }

    bool operator!=(const Invitation& invitation ) const {
        return invitationCode != invitation.toString();
    }
private:
    Invitation();
    explicit Invitation(const std::string& invitationCode);
    static unsigned long count;
    std::string SEPARATOR = "x";
    const size_t MINIMUM_COUNT_STRING_LENGTH = 3;
    const size_t RANDOM_CODE_LENGTH = 3;
    const size_t MINIMUM_LENGTH = MINIMUM_COUNT_STRING_LENGTH + RANDOM_CODE_LENGTH + SEPARATOR.length();
    std::string invitationCode;
    std::string generateInvitationCode();
};

#endif