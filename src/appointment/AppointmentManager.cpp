#include "hms/AppointmentManager.h"

#include <algorithm>

#include "hms/DoctorManager.h"
#include "hms/PatientManager.h"

namespace hms {

AppointmentManager::AppointmentManager(const PatientManager& patients, const DoctorManager& doctors)
    : patients_(patients), doctors_(doctors) {}

// TODO(OP Medha): implement booking rules using patients_.exists(), doctors_.worksAt() and isSlotFree().
Result<Appointment> AppointmentManager::book(Id, Id, const TimeSlot&, const std::string&) {
    return Result<Appointment>::failure("not implemented yet");
}
Status AppointmentManager::cancel(Id) { return Status::failure("not implemented yet"); }
Status AppointmentManager::reschedule(Id, const TimeSlot&) { return Status::failure("not implemented yet"); }
Status AppointmentManager::markCompleted(Id) { return Status::failure("not implemented yet"); }
bool AppointmentManager::isSlotFree(Id, const TimeSlot&) const { return false; }
std::optional<Appointment> AppointmentManager::findById(Id) const { return std::nullopt; }
std::vector<Appointment> AppointmentManager::forPatient(Id) const { return {}; }
std::vector<Appointment> AppointmentManager::forDoctor(Id) const { return {}; }
std::vector<Appointment> AppointmentManager::onDate(const std::string&) const { return {}; }

void AppointmentManager::replaceAll(std::vector<Appointment> items) {
    appointments_ = std::move(items);
    nextId_ = 1;
    for (const auto& a : appointments_) nextId_ = std::max(nextId_, a.id + 1);
}

}  // namespace hms
