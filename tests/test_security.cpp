#include "TestHelper.h"
#include "TestData.h"
#include "hms/Security.h"

int main() {
    const std::string stored = hms::hashPassword("fictional-pass-123");
    CHECK(stored != "fictional-pass-123");
    CHECK(hms::verifyPassword("fictional-pass-123", stored));
    CHECK(!hms::verifyPassword("wrong", stored));
    return TEST_RESULT();
}
