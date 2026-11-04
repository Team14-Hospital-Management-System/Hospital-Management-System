#include "TestHelper.h"
#include "TestData.h"
#include <sstream>
#include <filesystem>

#include "hms/AppContext.h"
#include "hms/Cli.h"

int main() {
    const std::string testDir = "test_cli_data_dir";
    std::error_code ec;
    std::filesystem::remove_all(testDir, ec);

    // 1. Basic exit
    {
        hms::AppContext ctx(testDir);
        std::istringstream in("0\n");
        std::ostringstream out;
        CHECK(hms::runCli(ctx, in, out) == 0);
        CHECK(out.str().find("Hospital Management System") != std::string::npos);
        CHECK(out.str().find("Goodbye.") != std::string::npos);
    }

    // 2. Invalid choice then exit
    {
        hms::AppContext ctx(testDir);
        std::istringstream in("99\n0\n");
        std::ostringstream out;
        CHECK(hms::runCli(ctx, in, out) == 0);
        CHECK(out.str().find("Invalid choice") != std::string::npos);
    }

    // 3. Billing menu open and exit
    {
        hms::AppContext ctx(testDir);
        std::istringstream in("4\n0\n0\n");
        std::ostringstream out;
        CHECK(hms::runCli(ctx, in, out) == 0);
        CHECK(out.str().find("Billing Management") != std::string::npos);
    }

    // 4. Admin menu open and exit
    {
        hms::AppContext ctx(testDir);
        std::istringstream in("6\n0\n0\n");
        std::ostringstream out;
        CHECK(hms::runCli(ctx, in, out) == 0);
        CHECK(out.str().find("Administration") != std::string::npos);
    }

    // 5. Admin menu user registration and login
    {
        hms::AppContext ctx(testDir);
        // Choice 6 (Admin) -> Choice 3 (Register) -> user1, pass1, role 1 (Admin) -> 0 (Back) -> 0 (Exit)
        std::istringstream in("6\n3\nadmin_test\nsecure_pass\n1\n0\n0\n");
        std::ostringstream out;
        CHECK(hms::runCli(ctx, in, out) == 0);
        CHECK(out.str().find("registered successfully") != std::string::npos);
    }

    std::filesystem::remove_all(testDir, ec);
    return TEST_RESULT();
}
