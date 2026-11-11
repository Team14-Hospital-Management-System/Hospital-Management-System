# SRS implementation and verification status

This matrix is a code-review snapshot for the current integration branch. A passing build proves compilation and the registered automated tests; it does not automatically prove every SRS acceptance criterion.

## Functional requirements

| SRS ID | Requirement | Implementation snapshot | Verification status |
|---|---|---|---|
| HMS-F-001 | Authorized user login | Username/password authentication; login rejects invalid credentials. | Automated auth tests; manual login flow still required. |
| HMS-F-002 | Reception staff register patients | Patient registration menu and manager. | Unit test exists; demo flow still required. |
| HMS-F-003 | Authorized users update patient details | Patient update manager/menu. | Unit test exists. |
| HMS-F-004 | Search by ID/name | Patient lookup by ID and case-insensitive name search. | Unit test exists. |
| HMS-F-005 | Admin adds doctors | Doctor manager/menu. | Unit test exists. |
| HMS-F-006 | Admin updates doctors | Doctor update menu/manager. | Unit test exists. |
| HMS-F-007 | Display available doctors | Availability flag and working hours are represented. | Unit test exists; manual flow still required. |
| HMS-F-008 | Staff creates appointments | Appointment booking manager/menu. | Unit test exists. |
| HMS-F-009 | Prevent double booking | Scheduled doctor/time slot collision rejected. | Unit test exists. |
| HMS-F-010 | Cancel appointments | Appointment cancellation. | Unit test exists. |
| HMS-F-011 | Reschedule appointments | Rescheduling validates availability and collisions. | Unit test exists. |
| HMS-F-012 | Doctors view their appointment schedule | Doctor appointment menu exists, but no account-to-doctor profile association is implemented yet. | Partial; must be completed before claiming role-scoped doctor access. |
| HMS-F-013 | Basic billing information | Invoice creation, payment status and outstanding balance. | Unit test exists. |
| HMS-F-014 | Patient/appointment reports | Patient, appointment-date and doctor schedule reports. | Unit test exists. |
| HMS-F-015 | Admin creates/disables accounts | First-admin bootstrap, admin-only user creation and account disabling. | Auth tests added; storage round-trip test added. |

## Non-functional and security requirements

| SRS ID | Requirement | Current status |
|---|---|---|
| HMS-NF-001 | Common operations respond within 3 seconds | Not measured in a dedicated performance test. |
| HMS-NF-002 | Availability during normal hospital hours | Not verified; this is a local desktop application. |
| HMS-NF-003 | Restrict patient information to authorized users | Module entry now requires login and applies role checks; patient and doctor self-service/profile scoping remains incomplete. |
| HMS-NF-004 | Data persists and recovers after restart | Local text-file persistence exists. Automated storage tests cover file round trips; full multi-module restart scenario remains to be demonstrated. |
| HMS-NF-005 | Clear menus and messages | Basic CLI menus exist; usability acceptance requires human evaluation. |
| HMS-SR-001 | Username/password authentication | Implemented; automated auth tests exist. |
| HMS-SR-002 | Role-based access for Patient, Doctor, Receptionist, Admin | Admin and Receptionist roles are implemented; Doctor mapping is not scoped to a doctor profile; Patient is not an account role yet. Partial. |
| HMS-SR-003 | Prevent unauthorized patient-record display | Login and module role gates are present; patient/doctor record-level scoping remains incomplete. Partial. |
| HMS-SR-004 | Validate patient and appointment input | Manager-level validation exists, but strict calendar/time validation and complete malformed-input coverage remain. Partial. |
| HMS-SR-005 | Log important administrative actions | User creation and account disabling append to local `audit.log`. Record deletion is not currently an application operation. Partial. |

## Automated CI baseline

The pre-security integration on main completed the CMake configure/build and all 11 registered CTest tests on both Ubuntu and Windows. The follow-up security/admin changes add regression tests and must pass CI on their exact PR head before merge.

## Known scope and safety limitations

- This is an academic C++ CLI project with local text-file storage, not a production clinical information system.
- Do not use real patient data. Local patient files are not encrypted.
- The current password hash implementation is a dependency-free educational implementation, not a production password-hashing standard such as Argon2id, scrypt, or PBKDF2.
- The SRS lists four roles; the application still needs explicit Patient account linkage and Doctor-to-Doctor-record linkage for complete record-level authorization.
- Do not claim the performance, availability, or usability non-functional requirements have passed without measured evidence.
