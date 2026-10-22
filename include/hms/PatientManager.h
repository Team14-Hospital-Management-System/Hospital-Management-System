#pragma once
// Owner: Nirav.  Includes: Status.h, Types.h
#include <optional>
#include <string>
#include <vector>

#include "hms/Status.h"
#include "hms/Types.h"

namespace hms {

class PatientManager {
public:
    // `draft.id` is ignored; the manager assigns the next id and returns the stored patient.
    Result<Patient> registerPatient(const Patient& draft);
    Status updatePatient(const Patient& updated);          // matched by updated.id

    bool exists(Id id) const;
    std::optional<Patient> findById(Id id) const;
    std::vector<Patient> searchByName(const std::string& text) const;   // case-insensitive substring

    const std::vector<Patient>& all() const { return patients_; }
    void replaceAll(std::vector<Patient> items);

private:
    std::vector<Patient> patients_;
    Id nextId_ = 1;
};

}  // namespace hms
