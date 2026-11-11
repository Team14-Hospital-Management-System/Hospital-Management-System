#include "TestHelper.h"
#include "TestData.h"
#include "hms/Auth.h"

int main() {
    hms::Auth auth;
    CHECK(!auth.isLoggedIn());
    CHECK(auth.all().empty());
    CHECK(!auth.login("nirav", "password").ok);
    CHECK(auth.registerUser("nirav", "correct horse", hms::Role::Admin).ok);
    CHECK(auth.all().size() == 1);
    CHECK(!auth.all()[0].passwordHash.empty());
    CHECK(auth.all()[0].passwordHash != "correct horse");
    CHECK(!auth.registerUser("NIRAV", "another", hms::Role::Doctor).ok);
    CHECK(!auth.login("nirav", "wrong").ok);
    CHECK(!auth.isLoggedIn());
    CHECK(auth.login("NIRAV", "correct horse").ok);
    CHECK(auth.isLoggedIn());
    CHECK(auth.currentUser().has_value());
    CHECK(auth.currentUser()->username == "nirav");
    CHECK(auth.hasRole(hms::Role::Admin));
    CHECK(!auth.hasRole(hms::Role::Doctor));
    CHECK(!auth.disableUser(1).ok); // a non-admin cannot disable accounts
    auth.logout();
    CHECK(!auth.isLoggedIn());
    CHECK(!auth.currentUser().has_value());

    hms::Auth restored;
    restored.replaceAll(auth.all());
    CHECK(restored.all().size() == 1);
    CHECK(restored.login("nirav", "correct horse").ok);
    CHECK(!restored.disableUser(1).ok); // cannot disable the currently signed-in admin
    restored.logout();
    CHECK(restored.login("nirav", "correct horse").ok);
    CHECK(restored.registerUser("reception", "test-pass", hms::Role::Receptionist).ok);
    restored.logout();
    CHECK(restored.login("nirav", "correct horse").ok);
    CHECK(restored.disableUser(2).ok);
    restored.logout();
    CHECK(!restored.login("reception", "test-pass").ok);
    return TEST_RESULT();
}
