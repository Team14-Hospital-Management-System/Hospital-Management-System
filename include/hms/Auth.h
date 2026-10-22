#pragma once
// Owner: Nirav.  Includes: Status.h, Types.h   (.cpp also includes Security.h)
#include <optional>
#include <string>
#include <vector>

#include "hms/Status.h"
#include "hms/Types.h"

namespace hms {

class Auth {
public:
    Status registerUser(const std::string& username, const std::string& password, Role role);
    Status login(const std::string& username, const std::string& password);
    void logout();

    bool isLoggedIn() const;
    std::optional<User> currentUser() const;
    bool hasRole(Role role) const;

    // Persistence hooks (the CLI calls these around FileHandler).
    const std::vector<User>& all() const { return users_; }
    void replaceAll(std::vector<User> items);

private:
    std::vector<User> users_;
    Id nextId_ = 1;
    Id currentUserId_ = kInvalidId;
};

}  // namespace hms
