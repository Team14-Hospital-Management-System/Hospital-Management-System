#include "TestHelper.h"
#include "TestData.h"
#include "hms/PatientManager.h"

// TODO(Nirav): test register, update, search, replaceAll.
int main() {
    hms::PatientManager pm;
    CHECK(pm.all().empty());
    return TEST_RESULT();
}
