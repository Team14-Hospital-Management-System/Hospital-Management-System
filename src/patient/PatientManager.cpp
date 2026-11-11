#include "hms/PatientManager.h"

#include <algorithm>
#include <cctype>
#include <utility>

namespace hms {
namespace {
std::string lowercase(const std::string& value) {
    std::string result = value;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}
}  // namespace

Result<Patient> PatientManager::registerPatient(const Patient& draft) {
    if (draft.name.empty()) return Result<Patient>::failure("Patient name cannot be empty.");
    if (draft.age < 0 || draft.age > 130) return Result<Patient>::failure("Patient age must be between 0 and 130.");

    Patient patient = draft;
    patient.id = nextId_++;
    patients_.push_back(patient);
    return Result<Patient>::success(std::move(patient));
}

Status PatientManager::updatePatient(const Patient& updated) {
    if (updated.id == kInvalidId) return Status::failure("A valid patient ID is required.");
    if (updated.name.empty()) return Status::failure("Patient name cannot be empty.");
    if (updated.age < 0 || updated.age > 130) return Status::failure("Patient age must be between 0 and 130.");

    const auto found = std::find_if(patients_.begin(), patients_.end(), [&](const Patient& patient) {
        return patient.id == updated.id;
    });
    if (found == patients_.end()) return Status::failure("Patient not found.");
    *found = updated;
    return Status::success();
}

bool PatientManager::exists(Id id) const {
    return std::any_of(patients_.begin(), patients_.end(), [&](const Patient& patient) { return patient.id == id; });
}

std::optional<Patient> PatientManager::findById(Id id) const {
    const auto found = std::find_if(patients_.begin(), patients_.end(), [&](const Patient& patient) {
        return patient.id == id;
    });
    if (found == patients_.end()) return std::nullopt;
    return *found;
}

std::vector<Patient> PatientManager::searchByName(const std::string& text) const {
    const auto query = lowercase(text);
    std::vector<Patient> matches;
    for (const auto& patient : patients_) {
        if (lowercase(patient.name).find(query) != std::string::npos) matches.push_back(patient);
    }
    return matches;
}

void PatientManager::replaceAll(std::vector<Patient> items) {
    patients_ = std::move(items);
    nextId_ = 1;
    for (const auto& patient : patients_) nextId_ = std::max(nextId_, patient.id + 1);
}

}  // namespace hms
