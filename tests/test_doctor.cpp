#include "TestHelper.h"
#include "TestData.h"
#include "hms/DoctorManager.h"

// TODO(Pranav Kaushal): test add, availability, worksAt.
int main() {
    hms::DoctorManager dm;
    CHECK(dm.all().empty());
    return TEST_RESULT();
}
