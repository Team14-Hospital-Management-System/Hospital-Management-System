#include "hms/Cli.h"

#include <istream>
#include <ostream>
#include <string>

#include "hms/AppContext.h"

namespace hms {
namespace {
bool allowed(const Auth& auth, Role role) { return auth.hasRole(Role::Admin) || auth.hasRole(role); }
}

int runCli(AppContext& ctx, std::istream& in, std::ostream& out) {
    const auto loaded = ctx.loadAll();
    if (!loaded.ok) out << "Warning: could not load saved data: " << loaded.message << "\n";

    std::string choice;
    while (true) {
        out << "\n=== Hospital Management System ===\n";
        if (!ctx.auth.isLoggedIn()) {
            out << "Not logged in. Select Administration to log in or create the first administrator.\n"
                   "6) Administration / Login\n0) Exit\n> ";
        } else {
            const auto user = ctx.auth.currentUser();
            out << "Signed in as " << (user ? user->username : "unknown")
                << " (" << (user ? toString(user->role) : "unknown") << ")\n"
                   "1) Patients\n2) Doctors\n3) Appointments\n4) Billing\n"
                   "5) Reports\n6) Administration\n0) Exit\n> ";
        }
        if (!std::getline(in, choice)) break;
        if (choice == "0") break;
        if (choice == "6") { runAdminMenu(ctx, in, out); continue; }
        if (!ctx.auth.isLoggedIn()) {
            out << "Access denied. Please log in first.\n";
            continue;
        }
        if (choice == "1" && allowed(ctx.auth, Role::Receptionist)) runPatientMenu(ctx, in, out);
        else if (choice == "2" && ctx.auth.hasRole(Role::Admin)) runDoctorMenu(ctx, in, out);
        else if (choice == "3" && (allowed(ctx.auth, Role::Receptionist) || ctx.auth.hasRole(Role::Doctor))) runAppointmentMenu(ctx, in, out);
        else if (choice == "4" && allowed(ctx.auth, Role::Receptionist)) runBillingMenu(ctx, in, out);
        else if (choice == "5" && ctx.auth.hasRole(Role::Admin)) runReportMenu(ctx, in, out);
        else if (choice == "1" || choice == "2" || choice == "3" || choice == "4" || choice == "5")
            out << "Access denied: your account role cannot open this module.\n";
        else out << "Invalid choice, please try again.\n";
    }
    out << "Goodbye.\n";
    const auto saved = ctx.saveAll();
    if (!saved.ok) out << "Warning: could not save all data: " << saved.message << "\n";
    return 0;
}

}  // namespace hms
