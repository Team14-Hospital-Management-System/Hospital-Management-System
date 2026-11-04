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

    // 5. Admin menu: view current user and list users when none registered
    {
        hms::AppContext ctx(testDir);
        // Choice 6 (Admin) -> Choice 4 (View current user) -> Choice 5 (List all) -> 0 (Back) -> 0 (Exit)
        std::istringstream in("6\n4\n5\n0\n0\n");
        std::ostringstream out;
        CHECK(hms::runCli(ctx, in, out) == 0);
        CHECK(out.str().find("No user is currently logged in.") != std::string::npos);
        CHECK(out.str().find("No users registered yet.") != std::string::npos);
    }

    // 6. Billing menu: view invoices and balance when empty
    {
        hms::AppContext ctx(testDir);
        // Choice 4 (Billing) -> Choice 6 (View all) -> Choice 5 (Outstanding for patient 1) -> 0 (Back) -> 0 (Exit)
        std::istringstream in("4\n6\n5\n1\n0\n0\n");
        std::ostringstream out;
        CHECK(hms::runCli(ctx, in, out) == 0);
        CHECK(out.str().find("No invoices recorded.") != std::string::npos);
        CHECK(out.str().find("Outstanding balance for patient 1: 0 cents") != std::string::npos);
    }

    std::filesystem::remove_all(testDir, ec);
    return TEST_RESULT();
}
