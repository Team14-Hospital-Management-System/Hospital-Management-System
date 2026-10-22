#include "hms/PatientManager.h"

#include <algorithm>

namespace hms {

// TODO(Nirav): implement registration, update, lookup and search.
Result<Patient> PatientManager::registerPatient(const Patient&) { return Result<Patient>::failure("not implemented yet"); }
Status PatientManager::updatePatient(const Patient&) { return Status::failure("not implemented yet"); }
bool PatientManager::exists(Id) const { return false; }
std::optional<Patient> PatientManager::findById(Id) const { return std::nullopt; }
std::vector<Patient> PatientManager::searchByName(const std::string&) const { return {}; }

void PatientManager::replaceAll(std::vector<Patient> items) {
    patients_ = std::move(items);
    nextId_ = 1;
    for (const auto& p : patients_) nextId_ = std::max(nextId_, p.id + 1);
}

}  // namespace hms
