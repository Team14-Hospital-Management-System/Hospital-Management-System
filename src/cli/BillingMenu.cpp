#include "hms/AppContext.h"
#include "hms/Cli.h"

#include <iostream>
#include <string>
#include <vector>

namespace hms {
namespace {

bool readId(std::istream& in, std::ostream& out, const std::string& prompt, Id& id) {
    out << prompt;
    std::string input;
    if (!std::getline(in, input)) return false;
    try {
        size_t pos = 0;
        int val = std::stoi(input, &pos);
        if (pos != input.size() || val <= 0) {
            out << "Enter a valid positive ID.\n";
            return false;
        }
        id = val;
        return true;
    } catch (...) {
        out << "Enter a valid positive ID.\n";
        return false;
    }
}

bool readText(std::istream& in, std::ostream& out, const std::string& prompt, std::string& value) {
    out << prompt;
    return static_cast<bool>(std::getline(in, value));
}

void printInvoice(const Invoice& inv, std::ostream& out) {
    out << "Invoice ID: " << inv.id
        << "\nPatient ID: " << inv.patientId
        << "\nAppointment ID: " << (inv.appointmentId == kInvalidId ? "None" : std::to_string(inv.appointmentId))
        << "\nAmount (cents): " << inv.amountCents
        << "\nDescription: " << inv.description
        << "\nStatus: " << toString(inv.status) << "\n";
}

}  // namespace

void runBillingMenu(AppContext& ctx, std::istream& in, std::ostream& out) {
    while (true) {
        out << "\n=== Billing Management ===\n"
               "1) Create invoice\n"
               "2) Mark invoice as paid\n"
               "3) View invoice by ID\n"
               "4) View invoices for patient\n"
               "5) View outstanding balance for patient\n"
               "6) View all invoices\n"
               "0) Back\n> ";
        std::string choice;
        if (!std::getline(in, choice) || choice == "0") return;

        if (choice == "1") {
            Id patientId = kInvalidId;
            if (!readId(in, out, "Patient ID: ", patientId)) continue;
            if (!ctx.patients.exists(patientId)) {
                out << "Patient ID " << patientId << " does not exist.\n";
                continue;
            }

            std::string apptStr;
            out << "Appointment ID (0 if none): ";
            if (!std::getline(in, apptStr)) return;
            Id apptId = kInvalidId;
            try {
                int parsed = std::stoi(apptStr);
                if (parsed > 0) apptId = parsed;
            } catch (...) {
                apptId = kInvalidId;
            }

            std::string amtStr;
            out << "Amount in cents (e.g. 5000 for $50.00): ";
            if (!std::getline(in, amtStr)) return;
            long long amount = 0;
            try {
                amount = std::stoll(amtStr);
            } catch (...) {
                out << "Invalid amount.\n";
                continue;
            }

            std::string desc;
            if (!readText(in, out, "Description: ", desc)) return;

            auto result = ctx.billing.createInvoice(patientId, apptId, amount, desc);
            if (result.ok()) {
                out << "Invoice created successfully. Assigned ID: " << result.value.id << "\n";
            } else {
                out << "Failed to create invoice: " << result.status.message << "\n";
            }
        } else if (choice == "2") {
            Id invoiceId = kInvalidId;
            if (!readId(in, out, "Invoice ID to mark as paid: ", invoiceId)) continue;
            auto status = ctx.billing.markPaid(invoiceId);
            if (status.ok) {
                out << "Invoice " << invoiceId << " marked as paid.\n";
            } else {
                out << "Failed: " << status.message << "\n";
            }
        } else if (choice == "3") {
            Id invoiceId = kInvalidId;
            if (!readId(in, out, "Invoice ID: ", invoiceId)) continue;
            auto inv = ctx.billing.findById(invoiceId);
            if (inv.has_value()) {
                printInvoice(*inv, out);
            } else {
                out << "Invoice not found.\n";
            }
        } else if (choice == "4") {
            Id patientId = kInvalidId;
            if (!readId(in, out, "Patient ID: ", patientId)) continue;
            auto list = ctx.billing.forPatient(patientId);
            if (list.empty()) {
                out << "No invoices found for patient ID " << patientId << ".\n";
            } else {
                for (const auto& inv : list) {
                    printInvoice(inv, out);
                    out << "--------------------\n";
                }
            }
        } else if (choice == "5") {
            Id patientId = kInvalidId;
            if (!readId(in, out, "Patient ID: ", patientId)) continue;
            long long cents = ctx.billing.outstandingCents(patientId);
            out << "Outstanding balance for patient " << patientId << ": " << cents << " cents\n";
        } else if (choice == "6") {
            const auto& all = ctx.billing.all();
            if (all.empty()) {
                out << "No invoices recorded.\n";
            } else {
                for (const auto& inv : all) {
                    printInvoice(inv, out);
                    out << "--------------------\n";
                }
            }
        } else {
            out << "Invalid choice, please try again.\n";
        }
    }
}

}  // namespace hms
