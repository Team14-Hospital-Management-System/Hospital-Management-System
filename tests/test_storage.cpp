#include "TestHelper.h"
#include "TestData.h"
#include <filesystem>
#include "hms/FileHandler.h"

int main() {
    const std::string testDir = "test_storage_data_dir";
    std::error_code ec;
    std::filesystem::remove_all(testDir, ec);

    hms::FileHandler fh(testDir);
    CHECK(fh.dataDir() == testDir);

    // 1. Loading from missing files returns success and empty vector
    {
        std::vector<hms::User> users;
        CHECK(fh.loadUsers(users).ok);
        CHECK(users.empty());

        std::vector<hms::Patient> patients;
        CHECK(fh.loadPatients(patients).ok);
        CHECK(patients.empty());

        std::vector<hms::Doctor> doctors;
        CHECK(fh.loadDoctors(doctors).ok);
        CHECK(doctors.empty());

        std::vector<hms::Appointment> appointments;
        CHECK(fh.loadAppointments(appointments).ok);
        CHECK(appointments.empty());

        std::vector<hms::Invoice> invoices;
        CHECK(fh.loadInvoices(invoices).ok);
        CHECK(invoices.empty());
    }

    // 2. ensureDataDir creates directory
    CHECK(fh.ensureDataDir().ok);
    CHECK(std::filesystem::exists(testDir));

    // 3. Users save and load round-trip
    {
        std::vector<hms::User> users = {
            {1, "admin", "hash1$abc", hms::Role::Admin},
            {2, "doctor_who", "hash2$def", hms::Role::Doctor},
            {3, "front_desk", "hash3$ghi", hms::Role::Receptionist}
        };
        users[1].enabled = false; // verify account-disable state survives restart
        CHECK(fh.saveUsers(users).ok);

        std::vector<hms::User> loaded;
        CHECK(fh.loadUsers(loaded).ok);
        CHECK(loaded.size() == 3);
        CHECK(loaded[0].id == 1 && loaded[0].username == "admin" && loaded[0].role == hms::Role::Admin);
        CHECK(loaded[1].id == 2 && loaded[1].username == "doctor_who" && loaded[1].role == hms::Role::Doctor);
        CHECK(loaded[2].id == 3 && loaded[2].username == "front_desk" && loaded[2].role == hms::Role::Receptionist);
        CHECK(loaded[0].enabled);
        CHECK(!loaded[1].enabled);
        CHECK(loaded[2].enabled);
    }

    // 4. Patients save and load round-trip
    {
        std::vector<hms::Patient> patients;
        hms::Patient p1 = testdata::patientOne();
        p1.id = 1;
        p1.medicalNotes = "Allergic to penicillin; requires follow-up | checked";
        hms::Patient p2 = testdata::patientTwo();
        p2.id = 2;
        patients.push_back(p1);
        patients.push_back(p2);

        CHECK(fh.savePatients(patients).ok);

        std::vector<hms::Patient> loaded;
        CHECK(fh.loadPatients(loaded).ok);
        CHECK(loaded.size() == 2);
        CHECK(loaded[0].id == 1);
        CHECK(loaded[0].name == p1.name);
        CHECK(loaded[0].address == p1.address);
        CHECK(loaded[0].medicalNotes == p1.medicalNotes);
        CHECK(loaded[1].id == 2);
        CHECK(loaded[1].name == p2.name);
    }

    // 5. Doctors save and load round-trip
    {
        std::vector<hms::Doctor> doctors;
        hms::Doctor d1 = testdata::doctorOne();
        d1.id = 1;
        d1.workStart = "08:30";
        d1.workEnd = "16:30";
        d1.available = true;

        hms::Doctor d2 = testdata::doctorTwo();
        d2.id = 2;
        d2.workStart = "10:00";
        d2.workEnd = "18:00";
        d2.available = false;

        doctors.push_back(d1);
        doctors.push_back(d2);

        CHECK(fh.saveDoctors(doctors).ok);

        std::vector<hms::Doctor> loaded;
        CHECK(fh.loadDoctors(loaded).ok);
        CHECK(loaded.size() == 2);
        CHECK(loaded[0].id == 1 && loaded[0].specialization == "General" && loaded[0].available == true);
        CHECK(loaded[1].id == 2 && loaded[1].specialization == "Cardiology" && loaded[1].available == false);
    }

    // 6. Appointments save and load round-trip
    {
        std::vector<hms::Appointment> appointments = {
            {1, 10, 20, {"2030-01-15", "10:00"}, "Regular checkup", hms::AppointmentStatus::Scheduled},
            {2, 11, 20, {"2030-01-15", "11:00"}, "Follow-up consultation", hms::AppointmentStatus::Completed},
            {3, 12, 21, {"2030-01-16", "14:00"}, "Cancelled slot", hms::AppointmentStatus::Cancelled}
        };

        CHECK(fh.saveAppointments(appointments).ok);

        std::vector<hms::Appointment> loaded;
        CHECK(fh.loadAppointments(loaded).ok);
        CHECK(loaded.size() == 3);
        CHECK(loaded[0].id == 1 && loaded[0].status == hms::AppointmentStatus::Scheduled);
        CHECK(loaded[1].id == 2 && loaded[1].status == hms::AppointmentStatus::Completed);
        CHECK(loaded[2].id == 3 && loaded[2].status == hms::AppointmentStatus::Cancelled);
    }

    // 7. Invoices save and load round-trip
    {
        std::vector<hms::Invoice> invoices = {
            {1, 10, 1, 5000, "Consultation fee", hms::InvoiceStatus::Unpaid},
            {2, 10, hms::kInvalidId, 2500, "Blood test lab", hms::InvoiceStatus::Paid}
        };

        CHECK(fh.saveInvoices(invoices).ok);

        std::vector<hms::Invoice> loaded;
        CHECK(fh.loadInvoices(loaded).ok);
        CHECK(loaded.size() == 2);
        CHECK(loaded[0].id == 1 && loaded[0].amountCents == 5000 && loaded[0].status == hms::InvoiceStatus::Unpaid);
        CHECK(loaded[1].id == 2 && loaded[1].amountCents == 2500 && loaded[1].status == hms::InvoiceStatus::Paid);
    }

    // Cleanup
    std::filesystem::remove_all(testDir, ec);
    return TEST_RESULT();
}
