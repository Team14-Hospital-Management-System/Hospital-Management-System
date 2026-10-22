#include "hms/Auth.h"

#include <algorithm>

#include "hms/Security.h"

namespace hms {

// TODO(Nirav): implement. Use hashPassword / verifyPassword from Security.h.
Status Auth::registerUser(const std::string&, const std::string&, Role) { return Status::failure("not implemented yet"); }
Status Auth::login(const std::string&, const std::string&) { return Status::failure("not implemented yet"); }
void Auth::logout() { currentUserId_ = kInvalidId; }
bool Auth::isLoggedIn() const { return currentUserId_ != kInvalidId; }
std::optional<User> Auth::currentUser() const { return std::nullopt; }
bool Auth::hasRole(Role) const { return false; }

void Auth::replaceAll(std::vector<User> items) {
    users_ = std::move(items);
    nextId_ = 1;
    for (const auto& u : users_) nextId_ = std::max(nextId_, u.id + 1);
}

}  // namespace hms
