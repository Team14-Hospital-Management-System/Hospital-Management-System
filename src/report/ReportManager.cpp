#include "hms/ReportManager.h"

#include "hms/AppointmentManager.h"
#include "hms/Billing.h"
#include "hms/DoctorManager.h"
#include "hms/PatientManager.h"

namespace hms {

ReportManager::ReportManager(const PatientManager& patients, const DoctorManager& doctors,
                             const AppointmentManager& appointments, const Billing& billing)
    : patients_(patients), doctors_(doctors), appointments_(appointments), billing_(billing) {}

// TODO(Pranav Kaushal): build the report strings from the read-only manager accessors.
Result<std::string> ReportManager::patientReport(Id) const { return Result<std::string>::failure("not implemented yet"); }
std::string ReportManager::appointmentReport(const std::string&) const { return "not implemented yet"; }
Result<std::string> ReportManager::doctorScheduleReport(Id) const { return Result<std::string>::failure("not implemented yet"); }

}  // namespace hms
