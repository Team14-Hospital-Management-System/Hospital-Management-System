#include <istream>
#include <ostream>
#include <string>
#include <limits>

#include "hms/AppContext.h"
#include "hms/Cli.h"

namespace hms {
namespace {
bool readLine(std::istream& in, std::ostream& out, const char* prompt, std::string& value) {
    out << prompt;
    return static_cast<bool>(std::getline(in, value));
}
void printPatient(const Patient& patient, std::ostream& out) {
    out << "ID: " << patient.id << "\nName: " << patient.name
        << "\nAge: " << patient.age << "\nGender: " << patient.gender
        << "\nPhone: " << patient.phone << "\nAddress: " << patient.address
        << "\nMedical notes: " << patient.medicalNotes << "\n";
}
bool readPatientFields(std::istream& in, std::ostream& out, Patient& patient, bool includeId) {
    std::string value;
    if (includeId) {
        if (!readLine(in, out, "Patient ID: ", value)) return false;
        try { patient.id = std::stoi(value); } catch (...) { out << "Invalid patient ID.\n"; return false; }
    }
    if (!readLine(in, out, "Name: ", patient.name)) return false;
    if (!readLine(in, out, "Age: ", value)) return false;
    try { patient.age = std::stoi(value); } catch (...) { out << "Invalid age.\n"; return false; }
    if (!readLine(in, out, "Gender: ", patient.gender)) return false;
    if (!readLine(in, out, "Phone: ", patient.phone)) return false;
    if (!readLine(in, out, "Address: ", patient.address)) return false;
    if (!readLine(in, out, "Medical notes: ", patient.medicalNotes)) return false;
    return true;
}
}  // namespace

void runPatientMenu(AppContext& ctx, std::istream& in, std::ostream& out) {
    std::string choice;
    while (true) {
        out << "\n=== Patient Management ===\n"
               "1) Register patient\n2) Search patients by name\n"
               "3) View patient by ID\n4) Update patient\n0) Back\n> ";
        if (!std::getline(in, choice) || choice == "0") return;

        if (choice == "1") {
            Patient draft;
            if (!readPatientFields(in, out, draft, false)) return;
            const auto result = ctx.patients.registerPatient(draft);
            if (result.ok()) out << "Patient registered successfully. Assigned ID: " << result.value.id << "\n";
            else out << "Registration failed: " << result.status.message << "\n";
        } else if (choice == "2") {
            std::string query;
            if (!readLine(in, out, "Name to search: ", query)) return;
            const auto matches = ctx.patients.searchByName(query);
            if (matches.empty()) out << "No matching patients found.\n";
            for (const auto& patient : matches) { printPatient(patient, out); out << "--------------------\n"; }
        } else if (choice == "3") {
            std::string value;
            if (!readLine(in, out, "Patient ID: ", value)) return;
            try {
                const auto patient = ctx.patients.findById(std::stoi(value));
                if (patient) printPatient(*patient, out);
                else out << "Patient not found.\n";
            } catch (...) { out << "Invalid patient ID.\n"; }
        } else if (choice == "4") {
            std::string value;
            if (!readLine(in, out, "Patient ID to update: ", value)) return;
            Patient updated;
            try {
                const auto existing = ctx.patients.findById(std::stoi(value));
                if (!existing) { out << "Patient not found.\n"; continue; }
                updated = *existing;
            } catch (...) { out << "Invalid patient ID.\n"; continue; }
            out << "Enter replacement details.\n";
            if (!readPatientFields(in, out, updated, false)) return;
            const auto status = ctx.patients.updatePatient(updated);
            out << (status.ok ? "Patient updated successfully.\n" : "Update failed: " + status.message + "\n");
        } else {
            out << "Invalid choice. Please try again.\n";
        }
    }
}

}  // namespace hms
