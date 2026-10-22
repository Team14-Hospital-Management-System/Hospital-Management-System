#pragma once
// Owner: Pranav Kaushal.  Header includes: Status.h, Types.h
// (forward-declares the four managers; the .cpp includes their headers). Read-only: never modifies data.
// Reports are returned as strings so they are easy to unit-test; the CLI prints them.
#include <string>

#include "hms/Status.h"
#include "hms/Types.h"

namespace hms {

class PatientManager;
class DoctorManager;
class AppointmentManager;
class Billing;

class ReportManager {
public:
    ReportManager(const PatientManager& patients, const DoctorManager& doctors,
                  const AppointmentManager& appointments, const Billing& billing);

    Result<std::string> patientReport(Id patientId) const;        // details + appointments + outstanding bill
    std::string appointmentReport(const std::string& date) const; // all appointments on "YYYY-MM-DD"
    Result<std::string> doctorScheduleReport(Id doctorId) const;

private:
    const PatientManager& patients_;
    const DoctorManager& doctors_;
    const AppointmentManager& appointments_;
    const Billing& billing_;
};

}  // namespace hms
