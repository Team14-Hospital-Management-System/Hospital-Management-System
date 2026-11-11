#include "hms/Auth.h"

#include <algorithm>
#include <cctype>
#include <utility>

#include "hms/Security.h"

namespace hms {
namespace {
std::string normalized(const std::string& value) {
    std::string result = value;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}
bool validRole(Role role) {
    return role == Role::Admin || role == Role::Receptionist || role == Role::Doctor;
}
}  // namespace

Status Auth::registerUser(const std::string& username, const std::string& password, Role role) {
    if (username.empty()) return Status::failure("Username cannot be empty.");
    if (password.empty()) return Status::failure("Password cannot be empty.");
    if (!validRole(role)) return Status::failure("Invalid user role.");

    const auto key = normalized(username);
    const auto duplicate = std::find_if(users_.begin(), users_.end(), [&](const User& user) {
        return normalized(user.username) == key;
    });
    if (duplicate != users_.end()) return Status::failure("Username already exists.");

    User user;
    user.id = nextId_++;
    user.username = username;
    user.passwordHash = hashPassword(password);
    user.role = role;
    users_.push_back(std::move(user));
    return Status::success();
}

Status Auth::login(const std::string& username, const std::string& password) {
    const auto key = normalized(username);
    const auto found = std::find_if(users_.begin(), users_.end(), [&](const User& user) {
        return normalized(user.username) == key;
    });
    if (found == users_.end() || !found->enabled || !verifyPassword(password, found->passwordHash)) {
        return Status::failure("Invalid username or password.");
    }
    currentUserId_ = found->id;
    return Status::success();
}

Status Auth::disableUser(Id userId) {
    if (!hasRole(Role::Admin)) return Status::failure("Only an administrator can disable accounts.");
    auto found = std::find_if(users_.begin(), users_.end(),
        [userId](const User& user) { return user.id == userId; });
    if (found == users_.end()) return Status::failure("User not found.");
    if (!found->enabled) return Status::failure("User account is already disabled.");
    if (found->id == currentUserId_) return Status::failure("You cannot disable your own account.");
    if (found->role == Role::Admin) {
        const auto activeAdmins = std::count_if(users_.begin(), users_.end(),
            [](const User& user) { return user.enabled && user.role == Role::Admin; });
        if (activeAdmins <= 1) return Status::failure("The last active administrator cannot be disabled.");
    }
    found->enabled = false;
    return Status::success();
}

void Auth::logout() { currentUserId_ = kInvalidId; }

bool Auth::isLoggedIn() const { return currentUserId_ != kInvalidId; }

std::optional<User> Auth::currentUser() const {
    const auto found = std::find_if(users_.begin(), users_.end(), [&](const User& user) {
        return user.id == currentUserId_;
    });
    if (found == users_.end()) return std::nullopt;
    return *found;
}

bool Auth::hasRole(Role role) const {
    const auto user = currentUser();
    return user.has_value() && user->role == role;
}

void Auth::replaceAll(std::vector<User> items) {
    users_ = std::move(items);
    nextId_ = 1;
    for (const auto& user : users_) nextId_ = std::max(nextId_, user.id + 1);
    if (currentUserId_ != kInvalidId &&
        std::none_of(users_.begin(), users_.end(), [&](const User& user) { return user.id == currentUserId_; })) {
        currentUserId_ = kInvalidId;
    }
}

}  // namespace hms
