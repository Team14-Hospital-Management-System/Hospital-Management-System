
#include <iostream>
#include <string>
#include <vector>

#include "hms/AppContext.h"
#include "hms/Cli.h"

namespace hms {
namespace {

bool readId(
    std::istream& in,
    std::ostream& out,
    const std::string& prompt,
    Id& id) {

    out << prompt;

    std::string input;
    if (!std::getline(in, input)) {
        return false;
    }

    try {
        std::size_t pos = 0;
        int value = std::stoi(input, &pos);

        if (pos != input.size() || value <= 0) {
            out << "Enter a valid positive ID.\n";
            return false;
        }

        id = value;
        return true;
    } catch (...) {
        out << "Enter a valid positive ID.\n";
        return false;
    }
}

bool readText(
    std::istream& in,
    std::ostream& out,
    const std::string& prompt,
    std::string& value) {

    out << prompt;
    return static_cast<bool>(std::getline(in, value));
}

void printAppointment(
    const Appointment& a,
    std::ostream& out) {

    out << "\nAppointment ID: " << a.id
        << "\nPatient ID: " << a.patientId
        << "\nDoctor ID: " << a.doctorId
        << "\nDate: " << a.slot.date
        << "\nTime: " << a.slot.time
        << "\nReason: " << a.reason
        << "\nStatus: " << toString(a.status)
        << "\n";
}

void printAppointments(
    const std::vector<Appointment>& appointments,
    std::ostream& out) {

    if (appointments.empty()) {
        out << "No appointments found.\n";
        return;
    }

    for (const auto& a : appointments) {
        printAppointment(a, out);
    }
}

}  // namespace

void runAppointmentMenu(
    AppContext& ctx,
    std::istream& in,
    std::ostream& out) {

    while (true) {
        out << "\n========== APPOINTMENT MANAGEMENT ==========\n"
            << "1. Book appointment\n"
            << "2. View all appointments\n"
            << "3. Cancel appointment\n"
            << "4. Reschedule appointment\n"
            << "5. Mark appointment as completed\n"
            << "6. Search by patient ID\n"
            << "7. Search by doctor ID\n"
            << "8. View appointments by date\n"
            << "0. Back to main menu\n"
            << "Enter choice: ";

        std::string choice;

        if (!std::getline(in, choice)) {
            return;
        }

        if (choice == "0") {
            return;
        }

        if (choice == "1") {
            Id patientId, doctorId;
            std::string date, time, reason;

            if (!readId(in, out, "Patient ID: ", patientId)) continue;
            if (!readId(in, out, "Doctor ID: ", doctorId)) continue;

            if (!readText(in, out, "Date (YYYY-MM-DD): ", date)) return;
            if (!readText(in, out, "Time (HH:MM): ", time)) return;
            if (!readText(in, out, "Reason: ", reason)) return;

            auto result = ctx.appointments.book(
                patientId, doctorId, TimeSlot{date, time}, reason);

            if (result.ok()) {
                out << "Appointment booked successfully!\n";
                printAppointment(result.value, out);
            } else {
                out << "Booking failed: "
                    << result.status.message << "\n";
            }
        }

        else if (choice == "2") {
            printAppointments(ctx.appointments.all(), out);
        }

        else if (choice == "3") {
            Id id;

            if (!readId(in, out, "Appointment ID to cancel: ", id)) {
                continue;
            }

            Status result = ctx.appointments.cancel(id);

            if (result.ok) {
                out << "Appointment cancelled successfully.\n";
            } else {
                out << "Cancellation failed: " << result.message << "\n";
            }
        }

        else if (choice == "4") {
            Id id;
            std::string date, time;

            if (!readId(in, out, "Appointment ID: ", id)) continue;
            if (!readText(in, out, "New date (YYYY-MM-DD): ", date)) return;
            if (!readText(in, out, "New time (HH:MM): ", time)) return;

            Status result = ctx.appointments.reschedule(
                id, TimeSlot{date, time});

            if (result.ok) {
                out << "Appointment rescheduled successfully.\n";
            } else {
                out << "Rescheduling failed: " << result.message << "\n";
            }
        }

        else if (choice == "5") {
            Id id;

            if (!readId(in, out, "Appointment ID: ", id)) continue;

            Status result = ctx.appointments.markCompleted(id);

            if (result.ok) {
                out << "Appointment marked as completed.\n";
            } else {
                out << "Operation failed: " << result.message << "\n";
            }
        }

        else if (choice == "6") {
            Id patientId;

            if (!readId(in, out, "Patient ID: ", patientId)) continue;

            printAppointments(
                ctx.appointments.forPatient(patientId), out);
        }

        else if (choice == "7") {
            Id doctorId;

            if (!readId(in, out, "Doctor ID: ", doctorId)) continue;

            printAppointments(
                ctx.appointments.forDoctor(doctorId), out);
        }

        else if (choice == "8") {
            std::string date;

            if (!readText(in, out, "Date (YYYY-MM-DD): ", date)) return;

            printAppointments(ctx.appointments.onDate(date), out);
        }

        else {
            out << "Invalid choice. Please try again.\n";
        }
    }
}

}  // namespace hms
