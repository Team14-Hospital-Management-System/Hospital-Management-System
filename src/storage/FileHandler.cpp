#include "hms/FileHandler.h"

namespace hms {

FileHandler::FileHandler(std::string dataDir) : dataDir_(std::move(dataDir)) {}

// TODO(Pranav Swaroop): implement text/CSV persistence. Missing file => success + empty vector.
Status FileHandler::ensureDataDir() const { return Status::failure("not implemented yet"); }
Status FileHandler::saveUsers(const std::vector<User>&) const { return Status::failure("not implemented yet"); }
Status FileHandler::loadUsers(std::vector<User>&) const { return Status::failure("not implemented yet"); }
Status FileHandler::savePatients(const std::vector<Patient>&) const { return Status::failure("not implemented yet"); }
Status FileHandler::loadPatients(std::vector<Patient>&) const { return Status::failure("not implemented yet"); }
Status FileHandler::saveDoctors(const std::vector<Doctor>&) const { return Status::failure("not implemented yet"); }
Status FileHandler::loadDoctors(std::vector<Doctor>&) const { return Status::failure("not implemented yet"); }
Status FileHandler::saveAppointments(const std::vector<Appointment>&) const { return Status::failure("not implemented yet"); }
Status FileHandler::loadAppointments(std::vector<Appointment>&) const { return Status::failure("not implemented yet"); }
Status FileHandler::saveInvoices(const std::vector<Invoice>&) const { return Status::failure("not implemented yet"); }
Status FileHandler::loadInvoices(std::vector<Invoice>&) const { return Status::failure("not implemented yet"); }

}  // namespace hms
