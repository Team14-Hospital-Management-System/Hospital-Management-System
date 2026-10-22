#pragma once
// Owner: Pranav Kaushal.  Includes: Status.h, Types.h
#include <optional>
#include <string>
#include <vector>

#include "hms/Status.h"
#include "hms/Types.h"

namespace hms {

class DoctorManager {
public:
    Result<Doctor> addDoctor(const Doctor& draft);          // draft.id ignored; id assigned here
    Status updateDoctor(const Doctor& updated);             // matched by updated.id
    Status setAvailability(Id doctorId, bool available);

    bool exists(Id id) const;
    std::optional<Doctor> findById(Id id) const;
    std::vector<Doctor> findBySpecialization(const std::string& text) const;

    // True if the doctor exists, is marked available, and slot.time is inside
    // [workStart, workEnd). AppointmentManager calls this before booking.
    bool worksAt(Id doctorId, const TimeSlot& slot) const;

    const std::vector<Doctor>& all() const { return doctors_; }
    void replaceAll(std::vector<Doctor> items);

private:
    std::vector<Doctor> doctors_;
    Id nextId_ = 1;
};

}  // namespace hms
