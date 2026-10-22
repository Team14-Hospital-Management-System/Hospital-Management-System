#include "TestHelper.h"
#include "TestData.h"
#include "hms/Auth.h"

// TODO(Nirav): test register, login success/failure, logout, roles.
int main() {
    hms::Auth auth;
    CHECK(!auth.isLoggedIn());
    CHECK(auth.all().empty());
    return TEST_RESULT();
}
