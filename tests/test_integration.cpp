#include "TestHelper.h"
#include "TestData.h"
#include "hms/AppContext.h"

// Cross-module tests: all modules wired together through AppContext.
// TODO(all): once modules are implemented, add: register patient + add doctor -> book -> double-book fails
//            -> cancel -> rebook succeeds -> invoice -> report -> save/load round trip.
int main() {
    hms::AppContext ctx("test_data_dir");
    CHECK(ctx.patients.all().empty());
    CHECK(ctx.appointments.all().empty());
    return TEST_RESULT();
}
