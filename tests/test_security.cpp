#include "TestHelper.h"
#include "TestData.h"
#include "hms/Security.h"

int main() {
    const std::string stored = hms::hashPassword("fictional-pass-123");
    CHECK(stored != "fictional-pass-123");
    CHECK(hms::verifyPassword("fictional-pass-123", stored));
    CHECK(!hms::verifyPassword("wrong", stored));

    // 1. Two hashes of same password have different salts and strings
    std::string stored2 = hms::hashPassword("fictional-pass-123");
    CHECK(stored != stored2);
    CHECK(hms::verifyPassword("fictional-pass-123", stored2));

    // 2. Empty password works
    std::string emptyHash = hms::hashPassword("");
    CHECK(hms::verifyPassword("", emptyHash));
    CHECK(!hms::verifyPassword("not-empty", emptyHash));

    // 3. Malformed stored hash fails safely
    CHECK(!hms::verifyPassword("pass", ""));
    CHECK(!hms::verifyPassword("pass", "nosaltorhash"));
    CHECK(!hms::verifyPassword("pass", "saltonly$"));
    CHECK(!hms::verifyPassword("pass", "$hashonly"));

    return TEST_RESULT();
}
