#pragma once
// Owner: Pranav Swaroop. The single place where all modules are created and wired together.
// Member order matters: members are constructed top to bottom, and some depend on earlier ones.
#include <string>
#include <utility>
#include <vector>

#include "hms/AppointmentManager.h"
#include "hms/Auth.h"
#include "hms/Billing.h"
#include "hms/DoctorManager.h"
#include "hms/FileHandler.h"
#include "hms/PatientManager.h"
#include "hms/ReportManager.h"
#include "hms/Status.h"

namespace hms {

struct AppContext {
    explicit AppContext(std::string dataDir = "data")
        : storage(std::move(dataDir)),
          appointments(patients, doctors),
          reports(patients, doctors, appointments, billing) {}

    AppContext(const AppContext&) = delete;             // managers hold references to each other
    AppContext& operator=(const AppContext&) = delete;

    Status loadAll() {
        std::vector<User> u;
        auto s = storage.loadUsers(u);
        if (!s.ok) return s;
        auth.replaceAll(std::move(u));

        std::vector<Patient> p;
        s = storage.loadPatients(p);
        if (!s.ok) return s;
        patients.replaceAll(std::move(p));

        std::vector<Doctor> d;
        s = storage.loadDoctors(d);
        if (!s.ok) return s;
        doctors.replaceAll(std::move(d));

        std::vector<Appointment> a;
        s = storage.loadAppointments(a);
        if (!s.ok) return s;
        appointments.replaceAll(std::move(a));

        std::vector<Invoice> inv;
        s = storage.loadInvoices(inv);
        if (!s.ok) return s;
        billing.replaceAll(std::move(inv));

        return Status::success();
    }

    Status saveAll() const {
        auto s = storage.ensureDataDir();
        if (!s.ok) return s;
        s = storage.saveUsers(auth.all());
        if (!s.ok) return s;
        s = storage.savePatients(patients.all());
        if (!s.ok) return s;
        s = storage.saveDoctors(doctors.all());
        if (!s.ok) return s;
        s = storage.saveAppointments(appointments.all());
        if (!s.ok) return s;
        s = storage.saveInvoices(billing.all());
        return s;
    }

    FileHandler storage;
    Auth auth;
    PatientManager patients;
    DoctorManager doctors;
    AppointmentManager appointments;
    Billing billing;
    ReportManager reports;
};

}  // namespace hms
