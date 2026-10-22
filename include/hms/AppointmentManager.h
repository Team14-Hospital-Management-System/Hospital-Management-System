#pragma once
// Owner: OP Medha.  Header includes: Status.h, Types.h  (forward-declares PatientManager, DoctorManager)
// The .cpp includes PatientManager.h and DoctorManager.h to validate ids and doctor hours.
#include <optional>
#include <string>
#include <vector>

#include "hms/Status.h"
#include "hms/Types.h"

namespace hms {

class PatientManager;
class DoctorManager;

class AppointmentManager {
public:
    AppointmentManager(const PatientManager& patients, const DoctorManager& doctors);

    // Fails if: patient/doctor unknown, doctor not working at that slot, or slot already taken.
    Result<Appointment> book(Id patientId, Id doctorId, const TimeSlot& slot, const std::string& reason);
    Status cancel(Id appointmentId);
    Status reschedule(Id appointmentId, const TimeSlot& newSlot);   // same double-booking rules
    Status markCompleted(Id appointmentId);

    // Double-booking rule: a doctor has at most ONE Scheduled appointment per exact slot.
    // Cancelled appointments do not block a slot.
    bool isSlotFree(Id doctorId, const TimeSlot& slot) const;

    std::optional<Appointment> findById(Id id) const;
    std::vector<Appointment> forPatient(Id patientId) const;
    std::vector<Appointment> forDoctor(Id doctorId) const;
    std::vector<Appointment> onDate(const std::string& date) const;

    const std::vector<Appointment>& all() const { return appointments_; }
    void replaceAll(std::vector<Appointment> items);

private:
    const PatientManager& patients_;
    const DoctorManager& doctors_;
    std::vector<Appointment> appointments_;
    Id nextId_ = 1;
};

}  // namespace hms
