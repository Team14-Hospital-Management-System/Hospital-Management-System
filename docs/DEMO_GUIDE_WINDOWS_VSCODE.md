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

Expected test summary on the current CI baseline: **11/11 tests passed**. After the authentication/admin-access changes in PR #8 are merged, use the updated CI result as the source of truth.

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

## 5. First launch and demo data

On a fresh data directory:
1. Choose **Administration / Login**.
2. Choose **Register user** to create the first administrator account.
3. Use a throwaway demo username and password. Do not use a personal or reused password.
4. Return to Administration and log in with that administrator.
5. Open Patient Management and register a fictional patient.
6. Open Doctor Management (Admin only) and add a fictional doctor with a working time such as 09:00–17:00.
7. Open Appointment Management and book an available slot for the patient and doctor.
8. Try booking the same doctor/time again; it should be rejected.
9. Open Billing and create an invoice for the patient.
10. Open Reports to show the patient report, appointment report, and doctor schedule.
11. Exit using 0 so the application saves the data.

Use only fictional patient and doctor data. Do not enter real patient health information.

## 6. Where data is stored

The default directory is `data/` beside the executable's working directory. It contains local text files for users, patients, doctors, appointments, and invoices. Administrative user creation/disable actions are written to `audit.log`.

These files are local demo storage, not encrypted production medical records. Do not commit the `data/` directory or share it publicly.

## 7. If something fails

- **'cmake' is not recognized**: install CMake and restart VS Code.
- **No C++ compiler found**: install the Visual Studio C++ workload and restart VS Code.
- **Executable path not found**: check whether CMake generated Visual Studio output (`build/src/Debug/hms.exe`) or a single-config output (`build/src/hms.exe`).
- **Tests fail**: copy the full output from `ctest --test-dir build -C Debug --output-on-failure`; do not merge code with a failing required CI check.
