#include "TestHelper.h"
#include "TestData.h"
#include "hms/Status.h"
#include "hms/Types.h"

int main() {
    CHECK(hms::Status::success().ok);
    CHECK(!hms::Status::failure("x").ok);
    CHECK(hms::Result<int>::success(5).value == 5);
    CHECK(testdata::slot10am() == testdata::slot10am());
    CHECK(testdata::slot10am() != testdata::slot11am());
    return TEST_RESULT();
}
