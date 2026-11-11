#include "hms/Security.h"

#include <cstdint>
#include <random>
#include <sstream>
#include <iomanip>

namespace hms {
namespace {

// Strengthened salted hash with multiple mixing rounds.
// Retains standard C++17 portability without external dependencies.
std::uint64_t mixHash(const std::string& salt, const std::string& password) {
    std::uint64_t h = 1469598103934665603ULL;
    const std::string combined = salt + password;
    for (int round = 0; round < 64; ++round) {
        for (unsigned char c : combined) {
            h ^= c;
            h *= 1099511628211ULL;
            h = (h << 13) | (h >> (64 - 13));
            h ^= static_cast<std::uint64_t>(round);
        }
    }
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
    const std::uint64_t r1 = static_cast<std::uint64_t>(rd());
    const std::uint64_t r2 = static_cast<std::uint64_t>(rd());
    const std::string salt = hex((r1 << 32) ^ r2);
    return salt + "$" + hex(mixHash(salt, password));
}

bool verifyPassword(const std::string& password, const std::string& stored) {
    const auto pos = stored.find('$');
    if (pos == std::string::npos || pos == 0 || pos + 1 >= stored.size()) {
        return false;
    }
    const std::string salt = stored.substr(0, pos);
    const std::string expected = hex(mixHash(salt, password));
    const std::string actual = stored.substr(pos + 1);

    if (expected.size() != actual.size()) {
        return false;
    }
    unsigned char diff = 0;
    for (size_t i = 0; i < expected.size(); ++i) {
        diff |= static_cast<unsigned char>(expected[i] ^ actual[i]);
    }
    return diff == 0;
}

}  // namespace hms
