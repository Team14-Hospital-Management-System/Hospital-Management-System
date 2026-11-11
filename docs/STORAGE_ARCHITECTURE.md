# Hospital Management System — Persistence Architecture

**Owner:** Pranav Swaroop (Team 14)  
**Implementation:** `src/storage/FileHandler.cpp`  
**Public Header:** `include/hms/FileHandler.h`

---

## 1. Overview & Architectural Contract

The `FileHandler` component provides an isolated, deterministic persistence layer for all core hospital management entities without introducing third-party database dependencies.

### Core Contract
1. **Decoupled Business Logic:** Managers (`PatientManager`, `DoctorManager`, `AppointmentManager`, `Billing`, `Auth`) do not depend on storage. Data is passed via standard C++ vectors.
2. **Missing-File Resilience:** Calling any `load*()` method against a non-existent file or directory is **not** an error; it succeeds and returns an empty dataset (`out.clear()`).
3. **Atomic Directory Management:** `ensureDataDir()` creates required directories recursively using `std::filesystem::create_directories` with `std::error_code`, preventing unhandled runtime exceptions.

---

## 2. File Organization

All entity files reside within the directory configured at runtime (`data/` by default):

| Entity | Storage File | Serialization Format | Fields |
|---|---|---|---|
| **Users** | `users.txt` | Pipe-delimited | `id \| username \| passwordHash \| role` |
| **Patients** | `patients.txt` | Pipe-delimited | `id \| name \| age \| gender \| phone \| address \| medicalNotes` |
| **Doctors** | `doctors.txt` | Pipe-delimited | `id \| name \| specialization \| workStart \| workEnd \| available` |
| **Appointments** | `appointments.txt` | Pipe-delimited | `id \| patientId \| doctorId \| date \| time \| reason \| status` |
| **Invoices** | `invoices.txt` | Pipe-delimited | `id \| patientId \| appointmentId \| amountCents \| description \| status` |

---

## 3. Delimiter Escaping Grammar

To prevent field corruption caused by user-entered punctuation (e.g. addresses like `Apt 4B | 123 Main St` or medical notes containing line breaks), an escape grammar is enforced:

* `\\` $\rightarrow$ `\\\\` (Escaped backslash)
* `|` $\rightarrow$ `\\|` (Escaped pipe delimiter)
* `\n` $\rightarrow$ `\\n` (Literal escaped newline)
* `\r` $\rightarrow$ `\\r` (Literal escaped carriage return)

When reading from disk, `splitRow()` tokenizes the row respecting escape sequences, and `unescapeField()` reconstructs the original string with 100% byte fidelity.

---

## 4. Lifecycle Integration

Storage lifecycle is orchestrated cleanly in `AppContext`:
* **On Application Start:** `ctx.loadAll()` loads all 5 data entities into their respective in-memory managers.
* **On Application Exit:** `ctx.saveAll()` ensures the data directory exists and flushes in-memory collections to disk.
