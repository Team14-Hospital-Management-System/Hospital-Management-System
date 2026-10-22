#include "TestHelper.h"
#include "TestData.h"
#include <sstream>

#include "hms/AppContext.h"
#include "hms/Cli.h"

// Drives the menu with scripted input. Add cases as each menu gets implemented.
int main() {
    hms::AppContext ctx("test_data_dir");
    std::istringstream in("0\n");
    std::ostringstream out;
    CHECK(hms::runCli(ctx, in, out) == 0);
    CHECK(out.str().find("Hospital Management System") != std::string::npos);
    return TEST_RESULT();
}
