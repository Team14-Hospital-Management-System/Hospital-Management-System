#pragma once
// Shared data structures. FROZEN: add fields at the END of a struct only,
// and only via a team-approved PR. Never rename or remove existing fields.
#include <string>

namespace hms {

using Id = int;                 // 1, 2, 3, ... assigned by the owning manager
constexpr Id kInvalidId = 0;    // "no id yet" / "not set"

enum class Role { Admin, Receptionist, Doctor };
enum class AppointmentStatus { Scheduled, Cancelled, Completed };
enum class InvoiceStatus { Unpaid, Paid };

// Dates/times are plain strings so there is no chrono/timezone complexity.
// date: "YYYY-MM-DD"   time: "HH:MM" (24h). Plain string comparison orders them correctly.
struct TimeSlot {
    std::string date;
    std::string time;
};
inline bool operator==(const TimeSlot& a, const TimeSlot& b) {
    return a.date == b.date && a.time == b.time;
}
inline bool operator!=(const TimeSlot& a, const TimeSlot& b) { return !(a == b); }

struct User {
    Id id = kInvalidId;
    std::string username;
    std::string passwordHash;   // produced by hms::hashPassword (Security.h) - never plain text
    Role role = Role::Receptionist;
};

struct Patient {
    Id id = kInvalidId;
    std::string name;
    int age = 0;
    std::string gender;
    std::string phone;
    std::string address;
    std::string medicalNotes;
};

struct Doctor {
    Id id = kInvalidId;
    std::string name;
    std::string specialization;
    std::string workStart = "09:00";   // inclusive
    std::string workEnd = "17:00";     // exclusive
    bool available = true;             // false = on leave / not taking appointments
};

struct Appointment {
    Id id = kInvalidId;
    Id patientId = kInvalidId;
    Id doctorId = kInvalidId;
    TimeSlot slot;
    std::string reason;
    AppointmentStatus status = AppointmentStatus::Scheduled;
};

struct Invoice {
    Id id = kInvalidId;
    Id patientId = kInvalidId;
    Id appointmentId = kInvalidId;     // kInvalidId if not tied to an appointment
    long long amountCents = 0;         // integer cents: avoids floating-point money bugs
    std::string description;
    InvoiceStatus status = InvoiceStatus::Unpaid;
};

inline std::string toString(Role r) {
    switch (r) {
        case Role::Admin: return "Admin";
        case Role::Receptionist: return "Receptionist";
        case Role::Doctor: return "Doctor";
    }
    return "Unknown";
}
inline std::string toString(AppointmentStatus s) {
    switch (s) {
        case AppointmentStatus::Scheduled: return "Scheduled";
        case AppointmentStatus::Cancelled: return "Cancelled";
        case AppointmentStatus::Completed: return "Completed";
    }
    return "Unknown";
}
inline std::string toString(InvoiceStatus s) {
    return s == InvoiceStatus::Paid ? "Paid" : "Unpaid";
}

}  // namespace hms
