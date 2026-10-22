#include "TestHelper.h"
#include "TestData.h"
#include "hms/AppointmentManager.h"
#include "hms/Billing.h"
#include "hms/DoctorManager.h"
#include "hms/PatientManager.h"
#include "hms/ReportManager.h"

// TODO(Pranav Kaushal): test report contents using fictional data.
int main() {
    hms::PatientManager pm;
    hms::DoctorManager dm;
    hms::AppointmentManager am(pm, dm);
    hms::Billing b;
    hms::ReportManager rm(pm, dm, am, b);
    CHECK(!rm.appointmentReport("2030-01-15").empty());
    return TEST_RESULT();
}
