#include "hms/AppContext.h"
#include "hms/Cli.h"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>

namespace hms {
namespace {

bool readLine(std::istream& in, std::ostream& out, const std::string& prompt, std::string& value) {
    out << prompt;
    return static_cast<bool>(std::getline(in, value));
}

void audit(const AppContext& ctx, const std::string& action, const std::string& target) {
    if (!ctx.storage.ensureDataDir().ok) return;
    const auto path = std::filesystem::path(ctx.storage.dataDir()) / "audit.log";
    std::ofstream log(path, std::ios::app);
    if (!log) return;
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto* actor = ctx.auth.currentUser();
    const std::string actorName = actor ? actor->username : "initial-setup";
    const std::tm* local = std::localtime(&now);
    if (local) log << std::put_time(local, "%Y-%m-%d %H:%M:%S");
    else log << "unknown-time";
    log << " | actor=" << actorName << " | action=" << action
        << " | target=" << target << '\n';
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
               "6) Disable user account\n"
               "0) Back\n> ";
        std::string choice;
        if (!std::getline(in, choice) || choice == "0") return;

        if (choice == "1") {
            std::string username, password;
            if (!readLine(in, out, "Username: ", username)) return;
            if (!readLine(in, out, "Password: ", password)) return;
            const auto status = ctx.auth.login(username, password);
            if (status.ok) out << "Login successful. Welcome, " << username << "!\n";
            else out << "Login failed: " << status.message << "\n";
        } else if (choice == "2") {
            if (!ctx.auth.isLoggedIn()) out << "No user is currently logged in.\n";
            else { ctx.auth.logout(); out << "Logged out successfully.\n"; }
        } else if (choice == "3") {
            // The first account bootstraps the system; all later account creation is admin-only.
            if (!ctx.auth.all().empty() && !ctx.auth.hasRole(Role::Admin)) {
                out << "Permission denied: Only Admin can register new users.\n";
                continue;
            }
            std::string username, password, roleStr;
            if (!readLine(in, out, "New username: ", username)) return;
            if (!readLine(in, out, "New password: ", password)) return;
            if (!readLine(in, out, "Role (1: Admin, 2: Receptionist, 3: Doctor): ", roleStr)) return;
            Role role;
            if (roleStr == "1" || roleStr == "Admin") role = Role::Admin;
            else if (roleStr == "2" || roleStr == "Receptionist") role = Role::Receptionist;
            else if (roleStr == "3" || roleStr == "Doctor") role = Role::Doctor;
            else { out << "Invalid role selected.\n"; continue; }

            const auto status = ctx.auth.registerUser(username, password, role);
            if (status.ok) {
                audit(ctx, "USER_CREATED", username);
                out << "User '" << username << "' registered successfully.\n";
            } else out << "Registration failed: " << status.message << "\n";
        } else if (choice == "4") {
            const auto current = ctx.auth.currentUser();
            if (current) out << "User ID: " << current->id
                             << "\nUsername: " << current->username
                             << "\nRole: " << toString(current->role) << "\n";
            else out << "No user is currently logged in.\n";
        } else if (choice == "5") {
            if (!ctx.auth.hasRole(Role::Admin)) {
                out << "Permission denied: Only Admin can view all users.\n";
                continue;
            }
            if (ctx.auth.all().empty()) out << "No users registered yet.\n";
            for (const auto& user : ctx.auth.all()) {
                out << "ID: " << user.id << " | Username: " << user.username
                    << " | Role: " << toString(user.role)
                    << " | Status: " << (user.enabled ? "Enabled" : "Disabled") << "\n";
            }
        } else if (choice == "6") {
            if (!ctx.auth.hasRole(Role::Admin)) {
                out << "Permission denied: Only Admin can disable accounts.\n";
                continue;
            }
            std::string idText;
            if (!readLine(in, out, "User ID to disable: ", idText)) return;
            try {
                std::size_t used = 0;
                const int id = std::stoi(idText, &used);
                if (used != idText.size() || id <= 0) { out << "Enter a valid positive ID.\n"; continue; }
                const auto target = std::find_if(ctx.auth.all().begin(), ctx.auth.all().end(),
                    [id](const User& user) { return user.id == id; });
                const std::string targetName = target == ctx.auth.all().end() ? idText : target->username;
                const auto status = ctx.auth.disableUser(id);
                if (status.ok) {
                    audit(ctx, "USER_DISABLED", targetName);
                    out << "User account disabled.\n";
                } else out << "Disable failed: " << status.message << "\n";
            } catch (...) { out << "Enter a valid positive ID.\n"; }
        } else out << "Invalid choice, please try again.\n";
    }
}

}  // namespace hms
