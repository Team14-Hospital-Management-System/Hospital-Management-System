#include "hms/DoctorManager.h"

#include <algorithm>

namespace hms {

// TODO(Pranav Kaushal): implement doctor management and availability.
Result<Doctor> DoctorManager::addDoctor(const Doctor&) { return Result<Doctor>::failure("not implemented yet"); }
Status DoctorManager::updateDoctor(const Doctor&) { return Status::failure("not implemented yet"); }
Status DoctorManager::setAvailability(Id, bool) { return Status::failure("not implemented yet"); }
bool DoctorManager::exists(Id) const { return false; }
std::optional<Doctor> DoctorManager::findById(Id) const { return std::nullopt; }
std::vector<Doctor> DoctorManager::findBySpecialization(const std::string&) const { return {}; }
bool DoctorManager::worksAt(Id, const TimeSlot&) const { return false; }

void DoctorManager::replaceAll(std::vector<Doctor> items) {
    doctors_ = std::move(items);
    nextId_ = 1;
    for (const auto& d : doctors_) nextId_ = std::max(nextId_, d.id + 1);
}

}  // namespace hms
