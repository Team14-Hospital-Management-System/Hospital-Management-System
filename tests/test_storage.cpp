#include "TestHelper.h"
#include "TestData.h"
#include "hms/FileHandler.h"

// TODO(Pranav Swaroop): write save -> load round-trip tests using a temp directory and fictional data.
int main() {
    hms::FileHandler fh("test_data_dir");
    CHECK(fh.dataDir() == "test_data_dir");
    return TEST_RESULT();
}
