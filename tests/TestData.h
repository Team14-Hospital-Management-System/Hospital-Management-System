#pragma once
// FICTIONAL sample data for tests. Never put real patient, doctor or staff data in this repo.
#include "hms/Types.h"

namespace testdata {

inline hms::Patient patientOne()  { hms::Patient p; p.name = "Test Patient One";  p.age = 30; p.gender = "F"; p.phone = "555-0100"; p.address = "1 Example Road, Testville"; return p; }
inline hms::Patient patientTwo()  { hms::Patient p; p.name = "Test Patient Two";  p.age = 45; p.gender = "M"; p.phone = "555-0101"; p.address = "2 Example Road, Testville"; return p; }
inline hms::Doctor  doctorOne()   { hms::Doctor d;  d.name = "Dr Test Alpha";     d.specialization = "General"; return d; }
inline hms::Doctor  doctorTwo()   { hms::Doctor d;  d.name = "Dr Test Beta";      d.specialization = "Cardiology"; return d; }
inline hms::TimeSlot slot10am()   { return {"2030-01-15", "10:00"}; }
inline hms::TimeSlot slot11am()   { return {"2030-01-15", "11:00"}; }

}  // namespace testdata
