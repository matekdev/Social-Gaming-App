#ifndef USERLIST_H
#define USERLIST_H

#include "User.h"
#include <optional>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <memory>

struct UserIdHash
{
    size_t operator()(const UserId& id) const { 
        return std::hash<uintptr_t>()(id.getId());
    } 
};

class UserManager {
public:
    UserManager();
    bool createUser(const UserId& id);
    bool destroyUser(const UserId& id);

    std::optional<std::weak_ptr<User>> getUserRef(const UserId& id);

    std::unordered_map<UserId, std::shared_ptr<User>, UserIdHash>::iterator begin();
    std::unordered_map<UserId, std::shared_ptr<User>, UserIdHash>::iterator end();
    size_t size() const noexcept;

private:
    std::unordered_map<UserId, std::shared_ptr<User>, UserIdHash> idToUserMap;

    std::optional<User> getUser(const UserId& id);
};

#endif