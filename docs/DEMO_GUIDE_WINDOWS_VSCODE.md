# HMS demo guide (Windows + VS Code)

This is a local C++17 command-line application. It does not require a database server, Supabase, an API key, or an internet connection after the repository and compiler are installed.

## 1. Install the build tools

Install:
- Visual Studio 2022 Build Tools (or Visual Studio 2022) with **Desktop development with C++**.
- CMake 3.20 or newer.
- VS Code.
- VS Code extensions: **C/C++** (Microsoft) and **CMake Tools** (Microsoft).

Restart VS Code after installing the compiler/CMake so PATH and toolchain discovery refresh.

## 2. Get the latest code

Open PowerShell:

```powershell
git clone https://github.com/Team14-Hospital-Management-System/Hospital-Management-System.git
cd Hospital-Management-System
code .
```

If the folder already exists:

```powershell
git checkout main
git pull origin main
code .
```

## 3. Configure, build, and test

Open the VS Code terminal (**Terminal > New Terminal**) in the repository root:

```powershell
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

The CMake test suite currently registers 11 CTest entries: common, security, storage, auth, patient, doctor, appointment, billing, report, cli, and integration. The latest verified GitHub Actions run on the security/admin PR head completed both Ubuntu and Windows jobs successfully. See [the CI run](https://github.com/Team14-Hospital-Management-System/Hospital-Management-System/actions/runs/42962221644) for the authoritative result. Your local test count should show 11 tests; do not assume the build passed until the commands finish on your machine.

## 4. Run the application

For the Visual Studio CMake generator:

```powershell
.\build\src\Debug\hms.exe
```

For a single-configuration generator such as MinGW Makefiles:

```powershell
.\build\src\hms.exe
```

You can supply a custom data directory as the first argument:

```powershell
.\build\src\Debug\hms.exe .\demo-data
```

## 5. First launch and demo flow

On a fresh data directory:
1. Open **Administration / Login**.
2. Register the first administrator. There are no default credentials.
3. Log in using the demo administrator account you just created.
4. Open Patient Management and register a fictional patient.
5. Open Doctor Management as Admin and add a fictional doctor with working hours such as 09:00–17:00.
6. Open Appointment Management and book an available slot for the patient and doctor.
7. Try booking the same doctor/time again; the conflicting booking should be rejected.
8. Open Billing and create an invoice for the patient; demonstrate the payment-status flow.
9. Open Reports as Admin and show the available reports.
10. Exit normally so the application can save its data.

Access is role-gated. Admin has access to all implemented modules. Receptionist can use patient, appointment, and billing modules. Doctor accounts can log in but are deliberately blocked from the general appointment menu until a doctor account can be safely linked to one specific doctor record. Reports and doctor administration are Admin-only. Do not demonstrate a module as a Doctor user if the current implementation denies that access.

Use only fictional patient and doctor data. Do not enter real patient health information.

## 6. Where data is stored

The application uses local text-file storage under its data directory (by default, `data/` relative to the process working directory). Administrative user creation and account disabling are appended to `audit.log`.

These files are local demo storage, not encrypted production medical records. Do not commit the `data/` directory or share it publicly.

## 7. If something fails

- **'cmake' is not recognized**: install CMake and restart VS Code.
- **No C++ compiler found**: install the Visual Studio C++ workload and restart VS Code.
- **Executable path not found**: check whether CMake generated Visual Studio output (`build/src/Debug/hms.exe`) or a single-config output (`build/src/hms.exe`).
- **Tests fail**: copy the complete output from `ctest --test-dir build -C Debug --output-on-failure`. Do not merge code with a failing required CI check.
- **Login fails after a previous demo**: the user database persists. Use the account created for that data directory, or run with a new empty demo data directory.

## 8. Honest scope statement for the presentation

This is an academic CLI prototype, not a production hospital information system. Patient login/personal record access and doctor-to-doctor-profile schedule scoping are not complete. Local text files are not encrypted, and the included password hashing is educational rather than production-grade. Performance, availability, and usability targets have not been formally measured. See [the SRS implementation status](SRS_IMPLEMENTATION_STATUS.md) for the detailed requirement matrix.
