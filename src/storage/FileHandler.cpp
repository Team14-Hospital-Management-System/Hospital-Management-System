#include "hms/FileHandler.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

namespace hms {
namespace {

std::string escapeField(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 8);
    for (char c : s) {
        if (c == '\\') out += "\\\\";
        else if (c == '|') out += "\\|";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else out += c;
    }
    return out;
}

std::string unescapeField(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            ++i;
            if (s[i] == 'n') out += '\n';
            else if (s[i] == 'r') out += '\r';
            else if (s[i] == '|') out += '|';
            else if (s[i] == '\\') out += '\\';
            else {
                out += '\\';
                out += s[i];
            }
        } else {
            out += s[i];
        }
    }
    return out;
}

std::vector<std::string> splitRow(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;
    bool inEscape = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (inEscape) {
            current += '\\';
            current += c;
            inEscape = false;
        } else if (c == '\\') {
            inEscape = true;
        } else if (c == '|') {
            fields.push_back(unescapeField(current));
            current.clear();
        } else {
            current += c;
        }
    }
    if (inEscape) current += '\\';
    fields.push_back(unescapeField(current));
    return fields;
}

Role parseRole(const std::string& s) {
    if (s == "Admin" || s == "0") return Role::Admin;
    if (s == "Doctor" || s == "2") return Role::Doctor;
    return Role::Receptionist;
}

AppointmentStatus parseAppointmentStatus(const std::string& s) {
    if (s == "Cancelled" || s == "1") return AppointmentStatus::Cancelled;
    if (s == "Completed" || s == "2") return AppointmentStatus::Completed;
    return AppointmentStatus::Scheduled;
}

InvoiceStatus parseInvoiceStatus(const std::string& s) {
    if (s == "Paid" || s == "1") return InvoiceStatus::Paid;
    return InvoiceStatus::Unpaid;
}

}  // namespace

FileHandler::FileHandler(std::string dataDir) : dataDir_(std::move(dataDir)) {}

Status FileHandler::ensureDataDir() const {
    if (dataDir_.empty()) return Status::success();
    std::error_code ec;
    std::filesystem::create_directories(dataDir_, ec);
    if (ec) {
        return Status::failure("Failed to create data directory: " + ec.message());
    }
    return Status::success();
}

Status FileHandler::saveUsers(const std::vector<User>& items) const {
    const auto dirStatus = ensureDataDir();
    if (!dirStatus.ok) return dirStatus;

    std::filesystem::path path = std::filesystem::path(dataDir_) / "users.txt";
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) {
        return Status::failure("Failed to open file for writing: " + path.string());
    }

    for (const auto& u : items) {
        out << u.id << "|"
            << escapeField(u.username) << "|"
            << escapeField(u.passwordHash) << "|"
            << toString(u.role) << "\n";
    }

    if (!out.good()) {
        return Status::failure("Error while writing users to: " + path.string());
    }
    return Status::success();
}

Status FileHandler::loadUsers(std::vector<User>& out) const {
    std::filesystem::path path = std::filesystem::path(dataDir_) / "users.txt";
    if (!std::filesystem::exists(path)) {
        out.clear();
        return Status::success();
    }

    std::ifstream in(path);
    if (!in.is_open()) {
        return Status::failure("Failed to open file for reading: " + path.string());
    }

    std::vector<User> items;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
        auto fields = splitRow(line);
        if (fields.size() < 4) {
            return Status::failure("Malformed line in " + path.string());
        }
        try {
            User u;
            u.id = std::stoi(fields[0]);
            u.username = fields[1];
            u.passwordHash = fields[2];
            u.role = parseRole(fields[3]);
            items.push_back(std::move(u));
        } catch (...) {
            return Status::failure("Failed parsing record in " + path.string());
        }
    }

    out = std::move(items);
    return Status::success();
}

Status FileHandler::savePatients(const std::vector<Patient>& items) const {
    const auto dirStatus = ensureDataDir();
    if (!dirStatus.ok) return dirStatus;

    std::filesystem::path path = std::filesystem::path(dataDir_) / "patients.txt";
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) {
        return Status::failure("Failed to open file for writing: " + path.string());
    }

    for (const auto& p : items) {
        out << p.id << "|"
            << escapeField(p.name) << "|"
            << p.age << "|"
            << escapeField(p.gender) << "|"
            << escapeField(p.phone) << "|"
            << escapeField(p.address) << "|"
            << escapeField(p.medicalNotes) << "\n";
    }

    if (!out.good()) {
        return Status::failure("Error while writing patients to: " + path.string());
    }
    return Status::success();
}

Status FileHandler::loadPatients(std::vector<Patient>& out) const {
    std::filesystem::path path = std::filesystem::path(dataDir_) / "patients.txt";
    if (!std::filesystem::exists(path)) {
        out.clear();
        return Status::success();
    }

    std::ifstream in(path);
    if (!in.is_open()) {
        return Status::failure("Failed to open file for reading: " + path.string());
    }

    std::vector<Patient> items;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
        auto fields = splitRow(line);
        if (fields.size() < 7) {
            return Status::failure("Malformed line in " + path.string());
        }
        try {
            Patient p;
            p.id = std::stoi(fields[0]);
            p.name = fields[1];
            p.age = std::stoi(fields[2]);
            p.gender = fields[3];
            p.phone = fields[4];
            p.address = fields[5];
            p.medicalNotes = fields[6];
            items.push_back(std::move(p));
        } catch (...) {
            return Status::failure("Failed parsing record in " + path.string());
        }
    }

    out = std::move(items);
    return Status::success();
}

Status FileHandler::saveDoctors(const std::vector<Doctor>& items) const {
    const auto dirStatus = ensureDataDir();
    if (!dirStatus.ok) return dirStatus;

    std::filesystem::path path = std::filesystem::path(dataDir_) / "doctors.txt";
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) {
        return Status::failure("Failed to open file for writing: " + path.string());
    }

    for (const auto& d : items) {
        out << d.id << "|"
            << escapeField(d.name) << "|"
            << escapeField(d.specialization) << "|"
            << escapeField(d.workStart) << "|"
            << escapeField(d.workEnd) << "|"
            << (d.available ? "1" : "0") << "\n";
    }

    if (!out.good()) {
        return Status::failure("Error while writing doctors to: " + path.string());
    }
    return Status::success();
}

Status FileHandler::loadDoctors(std::vector<Doctor>& out) const {
    std::filesystem::path path = std::filesystem::path(dataDir_) / "doctors.txt";
    if (!std::filesystem::exists(path)) {
        out.clear();
        return Status::success();
    }

    std::ifstream in(path);
    if (!in.is_open()) {
        return Status::failure("Failed to open file for reading: " + path.string());
    }

    std::vector<Doctor> items;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
        auto fields = splitRow(line);
        if (fields.size() < 6) {
            return Status::failure("Malformed line in " + path.string());
        }
        try {
            Doctor d;
            d.id = std::stoi(fields[0]);
            d.name = fields[1];
            d.specialization = fields[2];
            d.workStart = fields[3];
            d.workEnd = fields[4];
            d.available = (fields[5] == "1" || fields[5] == "true");
            items.push_back(std::move(d));
        } catch (...) {
            return Status::failure("Failed parsing record in " + path.string());
        }
    }

    out = std::move(items);
    return Status::success();
}

Status FileHandler::saveAppointments(const std::vector<Appointment>& items) const {
    const auto dirStatus = ensureDataDir();
    if (!dirStatus.ok) return dirStatus;

    std::filesystem::path path = std::filesystem::path(dataDir_) / "appointments.txt";
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) {
        return Status::failure("Failed to open file for writing: " + path.string());
    }

    for (const auto& a : items) {
        out << a.id << "|"
            << a.patientId << "|"
            << a.doctorId << "|"
            << escapeField(a.slot.date) << "|"
            << escapeField(a.slot.time) << "|"
            << escapeField(a.reason) << "|"
            << toString(a.status) << "\n";
    }

    if (!out.good()) {
        return Status::failure("Error while writing appointments to: " + path.string());
    }
    return Status::success();
}

Status FileHandler::loadAppointments(std::vector<Appointment>& out) const {
    std::filesystem::path path = std::filesystem::path(dataDir_) / "appointments.txt";
    if (!std::filesystem::exists(path)) {
        out.clear();
        return Status::success();
    }

    std::ifstream in(path);
    if (!in.is_open()) {
        return Status::failure("Failed to open file for reading: " + path.string());
    }

    std::vector<Appointment> items;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
        auto fields = splitRow(line);
        if (fields.size() < 7) {
            return Status::failure("Malformed line in " + path.string());
        }
        try {
            Appointment a;
            a.id = std::stoi(fields[0]);
            a.patientId = std::stoi(fields[1]);
            a.doctorId = std::stoi(fields[2]);
            a.slot.date = fields[3];
            a.slot.time = fields[4];
            a.reason = fields[5];
            a.status = parseAppointmentStatus(fields[6]);
            items.push_back(std::move(a));
        } catch (...) {
            return Status::failure("Failed parsing record in " + path.string());
        }
    }

    out = std::move(items);
    return Status::success();
}

Status FileHandler::saveInvoices(const std::vector<Invoice>& items) const {
    const auto dirStatus = ensureDataDir();
    if (!dirStatus.ok) return dirStatus;

    std::filesystem::path path = std::filesystem::path(dataDir_) / "invoices.txt";
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) {
        return Status::failure("Failed to open file for writing: " + path.string());
    }

    for (const auto& inv : items) {
        out << inv.id << "|"
            << inv.patientId << "|"
            << inv.appointmentId << "|"
            << inv.amountCents << "|"
            << escapeField(inv.description) << "|"
            << toString(inv.status) << "\n";
    }

    if (!out.good()) {
        return Status::failure("Error while writing invoices to: " + path.string());
    }
    return Status::success();
}

Status FileHandler::loadInvoices(std::vector<Invoice>& out) const {
    std::filesystem::path path = std::filesystem::path(dataDir_) / "invoices.txt";
    if (!std::filesystem::exists(path)) {
        out.clear();
        return Status::success();
    }

    std::ifstream in(path);
    if (!in.is_open()) {
        return Status::failure("Failed to open file for reading: " + path.string());
    }

    std::vector<Invoice> items;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
        auto fields = splitRow(line);
        if (fields.size() < 6) {
            return Status::failure("Malformed line in " + path.string());
        }
        try {
            Invoice inv;
            inv.id = std::stoi(fields[0]);
            inv.patientId = std::stoi(fields[1]);
            inv.appointmentId = std::stoi(fields[2]);
            inv.amountCents = std::stoll(fields[3]);
            inv.description = fields[4];
            inv.status = parseInvoiceStatus(fields[5]);
            items.push_back(std::move(inv));
        } catch (...) {
            return Status::failure("Failed parsing record in " + path.string());
        }
    }

    out = std::move(items);
    return Status::success();
}

}  // namespace hms
