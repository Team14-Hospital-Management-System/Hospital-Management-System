#include "TestHelper.h"
#include "TestData.h"
#include <set>
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
    CHECK(!hms::verifyPassword("pass", "salt$hash$extra"));

    // 4. Passwords with complex symbols and spaces
    const std::string complexPass = " P@$$w0rd_w!th_Sp@ces & $ymb0ls! #123 ";
    const std::string complexHash = hms::hashPassword(complexPass);
    CHECK(hms::verifyPassword(complexPass, complexHash));
    CHECK(!hms::verifyPassword(complexPass + " ", complexHash));

    // 5. Very long password (256 chars)
    const std::string longPass(256, 'x');
    const std::string longHash = hms::hashPassword(longPass);
    CHECK(hms::verifyPassword(longPass, longHash));
    CHECK(!hms::verifyPassword(std::string(255, 'x'), longHash));

    // 6. Salt entropy check (distinct salts across multiple calls)
    std::set<std::string> uniqueSalts;
    for (int i = 0; i < 10; ++i) {
        std::string h = hms::hashPassword("same-password");
        auto sep = h.find('$');
        CHECK(sep != std::string::npos);
        uniqueSalts.insert(h.substr(0, sep));
    }
    CHECK(uniqueSalts.size() == 10);

    return TEST_RESULT();
}
