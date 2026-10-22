#pragma once
// Owner: Pranav Swaroop.  Used by: AppContext / CLI only (no manager depends on storage).
// Includes: Status.h, Types.h
//
// Contract: load*() on a missing file is NOT an error - it returns success with an empty vector.
// Each entity type is stored in its own file inside dataDir (e.g. patients.txt).
#include <string>
#include <vector>

#include "hms/Status.h"
#include "hms/Types.h"

namespace hms {

class FileHandler {
public:
    explicit FileHandler(std::string dataDir);

    const std::string& dataDir() const { return dataDir_; }
    Status ensureDataDir() const;   // create the directory if it does not exist

    Status saveUsers(const std::vector<User>& items) const;
    Status loadUsers(std::vector<User>& out) const;

    Status savePatients(const std::vector<Patient>& items) const;
    Status loadPatients(std::vector<Patient>& out) const;

    Status saveDoctors(const std::vector<Doctor>& items) const;
    Status loadDoctors(std::vector<Doctor>& out) const;

    Status saveAppointments(const std::vector<Appointment>& items) const;
    Status loadAppointments(std::vector<Appointment>& out) const;

    Status saveInvoices(const std::vector<Invoice>& items) const;
    Status loadInvoices(std::vector<Invoice>& out) const;

private:
    std::string dataDir_;
};

}  // namespace hms
