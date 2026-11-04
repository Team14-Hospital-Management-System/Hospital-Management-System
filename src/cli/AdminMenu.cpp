#include "hms/AppContext.h"
#include "hms/Cli.h"

#include <iostream>
#include <string>

namespace hms {
namespace {

bool readLine(std::istream& in, std::ostream& out, const std::string& prompt, std::string& value) {
    out << prompt;
    return static_cast<bool>(std::getline(in, value));
}

}  // namespace

void runAdminMenu(AppContext& ctx, std::istream& in, std::ostream& out) {
    while (true) {
        out << "\n=== Administration ===\n"
               "1) Login\n"
               "2) Logout\n"
               "3) Register user\n"
               "4) View current user\n"
               "5) List all users\n"
               "0) Back\n> ";
        std::string choice;
        if (!std::getline(in, choice) || choice == "0") return;

        if (choice == "1") {
            std::string username, password;
            if (!readLine(in, out, "Username: ", username)) return;
            if (!readLine(in, out, "Password: ", password)) return;

            auto status = ctx.auth.login(username, password);
            if (status.ok) {
                out << "Login successful. Welcome, " << username << "!\n";
            } else {
                out << "Login failed: " << status.message << "\n";
            }
        } else if (choice == "2") {
            if (!ctx.auth.isLoggedIn()) {
                out << "No user is currently logged in.\n";
            } else {
                ctx.auth.logout();
                out << "Logged out successfully.\n";
            }
        } else if (choice == "3") {
            // First user can be registered without login; subsequent registrations require Admin role.
            if (!ctx.auth.all().empty() && !ctx.auth.hasRole(Role::Admin)) {
                out << "Permission denied: Only Admin can register new users.\n";
                continue;
            }

            std::string username, password, roleStr;
            if (!readLine(in, out, "New username: ", username)) return;
            if (!readLine(in, out, "New password: ", password)) return;
            if (!readLine(in, out, "Role (1: Admin, 2: Receptionist, 3: Doctor): ", roleStr)) return;

            Role role = Role::Receptionist;
            if (roleStr == "1" || roleStr == "Admin") role = Role::Admin;
            else if (roleStr == "2" || roleStr == "Receptionist") role = Role::Receptionist;
            else if (roleStr == "3" || roleStr == "Doctor") role = Role::Doctor;
            else {
                out << "Invalid role selected.\n";
                continue;
            }

            auto status = ctx.auth.registerUser(username, password, role);
            if (status.ok) {
                out << "User '" << username << "' registered successfully.\n";
            } else {
                out << "Registration failed: " << status.message << "\n";
            }
        } else if (choice == "4") {
            auto current = ctx.auth.currentUser();
            if (current.has_value()) {
                out << "User ID: " << current->id
                    << "\nUsername: " << current->username
                    << "\nRole: " << toString(current->role) << "\n";
            } else {
                out << "No user is currently logged in.\n";
            }
        } else if (choice == "5") {
            if (!ctx.auth.all().empty() && !ctx.auth.hasRole(Role::Admin)) {
                out << "Permission denied: Only Admin can view all users.\n";
                continue;
            }

            const auto& users = ctx.auth.all();
            if (users.empty()) {
                out << "No users registered yet.\n";
            } else {
                for (const auto& u : users) {
                    out << "ID: " << u.id
                        << " | Username: " << u.username
                        << " | Role: " << toString(u.role) << "\n";
                }
            }
        } else {
            out << "Invalid choice, please try again.\n";
        }
    }
}

}  // namespace hms
