#include "TestHelper.h"
#include "TestData.h"
#include "hms/Billing.h"

// TODO(Pranav Swaroop): test invoice creation, markPaid, outstandingCents.
int main() {
    hms::Billing b;
    CHECK(b.all().empty());
    return TEST_RESULT();
}
