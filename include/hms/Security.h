#pragma once
// Owner: Pranav Swaroop.  Used by: Auth.
// Includes: <string> only.
#include <string>

namespace hms {

// Returns a self-contained string (salt + hash) safe to store in User::passwordHash.
std::string hashPassword(const std::string& password);

// True if `password` matches a string previously produced by hashPassword.
bool verifyPassword(const std::string& password, const std::string& stored);

}  // namespace hms
