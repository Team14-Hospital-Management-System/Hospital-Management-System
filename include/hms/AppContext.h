#pragma once
// Owner: Pranav Swaroop. The single place where all modules are created and wired together.
// Member order matters: members are constructed top to bottom, and some depend on earlier ones.
#include <string>
#include <utility>

#include "hms/AppointmentManager.h"
#include "hms/Auth.h"
#include "hms/Billing.h"
#include "hms/DoctorManager.h"
#include "hms/FileHandler.h"
#include "hms/PatientManager.h"
#include "hms/ReportManager.h"

namespace hms {

struct AppContext {
    explicit AppContext(std::string dataDir = "data")
        : storage(std::move(dataDir)),
          appointments(patients, doctors),
          reports(patients, doctors, appointments, billing) {}

    AppContext(const AppContext&) = delete;             // managers hold references to each other
    AppContext& operator=(const AppContext&) = delete;

    FileHandler storage;
    Auth auth;
    PatientManager patients;
    DoctorManager doctors;
    AppointmentManager appointments;
    Billing billing;
    ReportManager reports;
};

}  // namespace hms
