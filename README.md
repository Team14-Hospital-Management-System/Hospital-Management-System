# Hospital Management System (C++17, CLI)

University software engineering project developed using C++17 with a command-line interface (CLI). The system provides a modular approach to managing hospital operations, including authentication, patient management, doctor management, appointment scheduling, report management, billing, administration, security, and storage. Each module is developed independently and integrated through predefined interfaces to ensure maintainability and consistency.

**All test and sample data must be fictional. Never commit real patient data or personally identifiable medical information.**




## Build and test

For the complete Windows + VS Code setup, first-run administrator creation, and demo flow, see [the demo guide](docs/DEMO_GUIDE_WINDOWS_VSCODE.md). The repository also includes VS Code tasks under `.vscode/tasks.json` for configure, build, test, and run. See [the SRS implementation status](docs/SRS_IMPLEMENTATION_STATUS.md) for verified features and remaining gaps, the [manual QA checklist](docs/MANUAL_QA_CHECKLIST.md) for repeatable demo tests, and [CONTRIBUTING.md](CONTRIBUTING.md) for the team PR workflow.


Ubuntu / Linux / macOS (needs CMake >= 3.20 and g++ or clang++):

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure
    ./build/src/hms            # run the app

Windows (PowerShell or Developer Command Prompt, Visual Studio 2019+ with C++ workload):

    cmake -S . -B build
    cmake --build build --config Debug
    ctest --test-dir build -C Debug --output-on-failure
    .\build\src\Debug\hms.exe

(Visual Studio is a multi-config generator, so it needs `--config` / `-C`. Single-config generators such as Ubuntu Makefiles ignore them.)
MinGW alternative: `cmake -S . -B build -G "MinGW Makefiles"`, then the Ubuntu commands.

## Ownership

| Module | Owner | Files |
|---|---|---|
| Auth, PatientManager | Nirav | `include/hms/Auth.h`, `PatientManager.h`, `src/auth/`, `src/patient/`, `src/cli/PatientMenu.cpp`, `tests/test_auth.cpp`, `test_patient.cpp` |
| DoctorManager, ReportManager | Pranav Kaushal | `DoctorManager.h`, `ReportManager.h`, `src/doctor/`, `src/report/`, `src/cli/DoctorMenu.cpp`, `ReportMenu.cpp`, matching tests |
| AppointmentManager | OP Medha | `AppointmentManager.h`, `src/appointment/`, `src/cli/AppointmentMenu.cpp`, `tests/test_appointment.cpp` |
| Billing, admin, security, storage, CLI core | Pranav Swaroop | `Billing.h`, `Security.h`, `FileHandler.h`, `AppContext.h`, `Cli.h`, `src/billing/`, `src/security/`, `src/storage/`, `src/cli/Cli.cpp`, `BillingMenu.cpp`, `AdminMenu.cpp`, `src/main.cpp`, matching tests |

## Rules that keep integration painless

1. **Interface freeze.** `Types.h`, `Status.h` and every public header in `include/hms/` are agreed contracts. Change one only through a PR that all four approve. Adding is fine; renaming or removing is not.
2. **Stay in your lane.** Edit only your own files and your own `src/<module>/CMakeLists.txt` (add new `.cpp` files there).
3. **Include only what the header table allows.** Managers never include each other's `.cpp`-private code. Only AppointmentManager and ReportManager use other managers, and only through the constructor references declared in their headers.
4. **No global state.** All modules are created in `AppContext`; menus receive it as a parameter.
5. **Every stub keeps compiling.** Replace the `not implemented yet` bodies with real logic without changing signatures.
6. **Branch per module, PR into `main`, CI must be green before merging.**
