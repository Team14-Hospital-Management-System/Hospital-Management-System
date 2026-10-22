#include "hms/Security.h"

#include <cstdint>
#include <random>
#include <sstream>
#include <iomanip>

namespace hms {
namespace {
// PLACEHOLDER for coursework: salted FNV-1a. NOT cryptographically secure.
// Pranav Swaroop: replace/strengthen as part of the security work, keeping these two signatures.
std::uint64_t fnv1a(const std::string& s) {
    std::uint64_t h = 1469598103934665603ULL;
    for (unsigned char c : s) { h ^= c; h *= 1099511628211ULL; }
    return h;
}
std::string hex(std::uint64_t v) {
    std::ostringstream o;
    o << std::hex << std::setw(16) << std::setfill('0') << v;
    return o.str();
}
}  // namespace

std::string hashPassword(const std::string& password) {
    std::random_device rd;
    std::string salt = hex((static_cast<std::uint64_t>(rd()) << 32) ^ rd());
    return salt + "$" + hex(fnv1a(salt + password));
}

bool verifyPassword(const std::string& password, const std::string& stored) {
    const auto pos = stored.find('$');
    if (pos == std::string::npos) return false;
    const std::string salt = stored.substr(0, pos);
    return stored.substr(pos + 1) == hex(fnv1a(salt + password));
}

}  // namespace hms
