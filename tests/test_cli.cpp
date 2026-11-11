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

    // Basic exit without any configured users.
    {
        hms::AppContext ctx(testDir);
        std::istringstream in("0\n");
        std::ostringstream out;
        CHECK(hms::runCli(ctx, in, out) == 0);
        CHECK(out.str().find("Hospital Management System") != std::string::npos);
        CHECK(out.str().find("Goodbye.") != std::string::npos);
    }

    // Unauthenticated users cannot open protected modules.
    {
        std::filesystem::remove_all(testDir, ec);
        hms::AppContext ctx(testDir);
        std::istringstream in("1\n0\n");
        std::ostringstream out;
        CHECK(hms::runCli(ctx, in, out) == 0);
        CHECK(out.str().find("Access denied. Please log in first.") != std::string::npos);
    }

    // Invalid choice remains handled.
    {
        std::filesystem::remove_all(testDir, ec);
        hms::AppContext ctx(testDir);
        std::istringstream in("99\n0\n");
        std::ostringstream out;
        CHECK(hms::runCli(ctx, in, out) == 0);
        CHECK(out.str().find("Access denied. Please log in first.") != std::string::npos);
    }

    // First-run administrator bootstrap and login.
    {
        std::filesystem::remove_all(testDir, ec);
        hms::AppContext ctx(testDir);
        std::istringstream in("6\n3\nadmin\nadmin-pass\n1\n0\n6\n1\nadmin\nadmin-pass\n0\n0\n");
        std::ostringstream out;
        CHECK(hms::runCli(ctx, in, out) == 0);
        CHECK(out.str().find("registered successfully") != std::string::npos);
        CHECK(out.str().find("Login successful") != std::string::npos);
    }

    // Receptionists may access patient records; doctors/admin-only modules are blocked.
    {
        std::filesystem::remove_all(testDir, ec);
        {
            hms::AppContext seed(testDir);
            CHECK(seed.auth.registerUser("frontdesk", "front-pass", hms::Role::Receptionist).ok);
            CHECK(seed.saveAll().ok);
        }
        hms::AppContext ctx(testDir);
        std::istringstream in("6\n1\nfrontdesk\nfront-pass\n0\n1\n0\n2\n0\n0\n");
        std::ostringstream out;
        CHECK(hms::runCli(ctx, in, out) == 0);
        CHECK(out.str().find("Patient Management") != std::string::npos);
        CHECK(out.str().find("cannot open this module") != std::string::npos);
    }

    // Doctor accounts are not granted the broad appointment menu until doctor-profile scoping exists.
    {
        std::filesystem::remove_all(testDir, ec);
        {
            hms::AppContext seed(testDir);
            CHECK(seed.auth.registerUser("doctor", "doctor-pass", hms::Role::Doctor).ok);
            CHECK(seed.saveAll().ok);
        }
        hms::AppContext ctx(testDir);
        std::istringstream in("6\\n1\\ndoctor\\ndoctor-pass\\n0\\n3\\n0\\n");
        std::ostringstream out;
        CHECK(hms::runCli(ctx, in, out) == 0);
        CHECK(out.str().find("Access denied: your account role cannot open this module.") != std::string::npos);
    }

    std::filesystem::remove_all(testDir, ec);
    return TEST_RESULT();
}
