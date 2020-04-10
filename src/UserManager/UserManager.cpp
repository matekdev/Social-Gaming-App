#include "UserManager.h"

UserManager::UserManager(): idToUserMap(std::unordered_map<UserId, std::shared_ptr<User>, UserIdHash>()) {};

bool UserManager::createUser(const UserId& id) {
    auto user = getUser(id);

    if (user.has_value()) {
        std::cout << " Error, user is already in UserManager." << '\n';
        return false;
    }

    idToUserMap.insert(std::make_pair(id, std::make_shared<User>(User(id))));
    return true;
}

bool UserManager::destroyUser(const UserId& id) {
    auto user = getUser(id);

    if (!user.has_value()) {
        std::cout << " Error, user does not exist in the UserManager." << '\n';
        return false;
    }

    idToUserMap.erase(id);
    return true;
}

std::optional<User> UserManager::getUser(const UserId& id) {
    auto userRef = getUserRef(id);
    if (!userRef.has_value()){
        return std::nullopt;
    }
    return std::make_optional(*userRef.value().lock());
}

std::optional<std::weak_ptr<User>> UserManager::getUserRef(const UserId& id) {
    auto iterator = idToUserMap.find(id);
    if (iterator == idToUserMap.end()) {
        return std::nullopt;
    }

    return std::make_optional(std::weak_ptr(iterator->second));
}

std::unordered_map<UserId, std::shared_ptr<User>, UserIdHash>::iterator UserManager::begin() {
    return idToUserMap.begin();
}

std::unordered_map<UserId, std::shared_ptr<User>, UserIdHash>::iterator UserManager::end() {
    return idToUserMap.end();
}

size_t UserManager::size() const noexcept {
    return idToUserMap.size();
}
