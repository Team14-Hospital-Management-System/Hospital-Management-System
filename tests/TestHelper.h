#pragma once
// Tiny test helper: no external framework needed.
//   CHECK(cond);        records a failure (with file:line) but keeps running
//   return TEST_RESULT();   at the end of main(): 0 = pass, 1 = fail (CTest reads this)
#include <iostream>

namespace hms_test {
inline int& failures() { static int f = 0; return f; }
}

#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::cerr << __FILE__ << ":" << __LINE__ << "  CHECK failed: " #cond "\n"; \
            ++hms_test::failures();                                              \
        }                                                                        \
    } while (0)

#define TEST_RESULT() (hms_test::failures() == 0 ? 0 : 1)
