#include "hms/Cli.h"

#include <istream>
#include <ostream>
#include <string>

#include "hms/AppContext.h"

namespace hms {

int runCli(AppContext& ctx, std::istream& in, std::ostream& out) {
    // Load existing data from storage (missing files succeed with empty data)
    ctx.loadAll();

    std::string choice;
    while (true) {
        out << "\n=== Hospital Management System ===\n"
               "1) Patients\n2) Doctors\n3) Appointments\n4) Billing\n5) Reports\n6) Administration\n0) Exit\n> ";
        if (!std::getline(in, choice)) break;   // end of input
        if (choice == "0") break;
        else if (choice == "1") runPatientMenu(ctx, in, out);
        else if (choice == "2") runDoctorMenu(ctx, in, out);
        else if (choice == "3") runAppointmentMenu(ctx, in, out);
        else if (choice == "4") runBillingMenu(ctx, in, out);
        else if (choice == "5") runReportMenu(ctx, in, out);
        else if (choice == "6") runAdminMenu(ctx, in, out);
        else out << "Invalid choice, please try again.\n";
    }
    out << "Goodbye.\n";

    // Save state on exit
    ctx.saveAll();
    return 0;
}

}  // namespace hms
