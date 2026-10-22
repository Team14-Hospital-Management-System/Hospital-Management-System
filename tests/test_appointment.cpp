#include "TestHelper.h"
#include "TestData.h"
#include "hms/AppointmentManager.h"
#include "hms/DoctorManager.h"
#include "hms/PatientManager.h"

// TODO(OP Medha): test booking, cancel, reschedule and double-booking prevention.
int main() {
    hms::PatientManager pm;
    hms::DoctorManager dm;
    hms::AppointmentManager am(pm, dm);
    CHECK(am.all().empty());
    return TEST_RESULT();
}
