#pragma once
// Owner: Pranav Swaroop (main menu). Each module owner writes their own menu function in src/cli/<Name>Menu.cpp.
// All menus read from `in` and write to `out` (not std::cin/std::cout directly) so tests can drive them.
#include <iosfwd>

namespace hms {

struct AppContext;

int runCli(AppContext& ctx, std::istream& in, std::ostream& out);   // main loop; returns process exit code

void runPatientMenu(AppContext& ctx, std::istream& in, std::ostream& out);      // Nirav
void runDoctorMenu(AppContext& ctx, std::istream& in, std::ostream& out);       // Pranav Kaushal
void runAppointmentMenu(AppContext& ctx, std::istream& in, std::ostream& out);  // OP Medha
void runBillingMenu(AppContext& ctx, std::istream& in, std::ostream& out);      // Pranav Swaroop
void runReportMenu(AppContext& ctx, std::istream& in, std::ostream& out);       // Pranav Kaushal
void runAdminMenu(AppContext& ctx, std::istream& in, std::ostream& out);        // Pranav Swaroop (users, login)

}  // namespace hms
