
#include "hms/AppointmentManager.h"

#include <algorithm>
#include <utility>

#include "hms/DoctorManager.h"
#include "hms/PatientManager.h"

namespace hms {

AppointmentManager::AppointmentManager(
    const PatientManager& patients,
    const DoctorManager& doctors)
    : patients_(patients), doctors_(doctors) {}

Result<Appointment> AppointmentManager::book(
    Id patientId,
    Id doctorId,
    const TimeSlot& slot,
    const std::string& reason) {

    if (!patients_.exists(patientId)) {
        return Result<Appointment>::failure("Patient does not exist");
    }

    if (!doctors_.exists(doctorId)) {
        return Result<Appointment>::failure("Doctor does not exist");
    }

    if (!doctors_.worksAt(doctorId, slot)) {
        return Result<Appointment>::failure(
            "Doctor is unavailable at the requested time");
    }

    if (!isSlotFree(doctorId, slot)) {
        return Result<Appointment>::failure(
            "Doctor already has an appointment at this time");
    }

    Appointment appointment;
    appointment.id = nextId_++;
    appointment.patientId = patientId;
    appointment.doctorId = doctorId;
    appointment.slot = slot;
    appointment.reason = reason;
    appointment.status = AppointmentStatus::Scheduled;

    appointments_.push_back(appointment);
    return Result<Appointment>::success(appointment);
}

Status AppointmentManager::cancel(Id appointmentId) {
    auto it = std::find_if(
        appointments_.begin(), appointments_.end(),
        [appointmentId](const Appointment& a) {
            return a.id == appointmentId;
        });

    if (it == appointments_.end()) {
        return Status::failure("Appointment not found");
    }

    if (it->status != AppointmentStatus::Scheduled) {
        return Status::failure(
            "Only scheduled appointments can be cancelled");
    }

    it->status = AppointmentStatus::Cancelled;
    return Status::success();
}

Status AppointmentManager::reschedule(
    Id appointmentId, const TimeSlot& newSlot) {

    auto it = std::find_if(
        appointments_.begin(), appointments_.end(),
        [appointmentId](const Appointment& a) {
            return a.id == appointmentId;
        });

    if (it == appointments_.end()) {
        return Status::failure("Appointment not found");
    }

    if (it->status != AppointmentStatus::Scheduled) {
        return Status::failure(
            "Only scheduled appointments can be rescheduled");
    }

    if (!doctors_.worksAt(it->doctorId, newSlot)) {
        return Status::failure(
            "Doctor is unavailable at the requested time");
    }

    const bool conflict = std::any_of(
        appointments_.begin(), appointments_.end(),
        [&](const Appointment& other) {
            return other.id != appointmentId &&
                   other.doctorId == it->doctorId &&
                   other.slot == newSlot &&
                   other.status == AppointmentStatus::Scheduled;
        });

    if (conflict) {
        return Status::failure(
            "Doctor already has an appointment at this time");
    }

    it->slot = newSlot;
    return Status::success();
}

Status AppointmentManager::markCompleted(Id appointmentId) {
    auto it = std::find_if(
        appointments_.begin(), appointments_.end(),
        [appointmentId](const Appointment& a) {
            return a.id == appointmentId;
        });

    if (it == appointments_.end()) {
        return Status::failure("Appointment not found");
    }

    if (it->status != AppointmentStatus::Scheduled) {
        return Status::failure(
            "Only scheduled appointments can be completed");
    }

    it->status = AppointmentStatus::Completed;
    return Status::success();
}

bool AppointmentManager::isSlotFree(
    Id doctorId, const TimeSlot& slot) const {

    return std::none_of(
        appointments_.begin(), appointments_.end(),
        [&](const Appointment& a) {
            return a.doctorId == doctorId &&
                   a.slot == slot &&
                   a.status == AppointmentStatus::Scheduled;
        });
}

std::optional<Appointment> AppointmentManager::findById(Id id) const {
    auto it = std::find_if(
        appointments_.begin(), appointments_.end(),
        [id](const Appointment& a) {
            return a.id == id;
        });

    if (it == appointments_.end()) {
        return std::nullopt;
    }

    return *it;
}

std::vector<Appointment> AppointmentManager::forPatient(
    Id patientId) const {

    std::vector<Appointment> result;

    for (const auto& a : appointments_) {
        if (a.patientId == patientId) {
            result.push_back(a);
        }
    }

    return result;
}

std::vector<Appointment> AppointmentManager::forDoctor(Id doctorId) const {
    std::vector<Appointment> result;

    for (const auto& a : appointments_) {
        if (a.doctorId == doctorId) {
            result.push_back(a);
        }
    }

    return result;
}

std::vector<Appointment> AppointmentManager::onDate(
    const std::string& date) const {

    std::vector<Appointment> result;

    for (const auto& a : appointments_) {
        if (a.slot.date == date) {
            result.push_back(a);
        }
    }

    return result;
}

void AppointmentManager::replaceAll(std::vector<Appointment> items) {
    appointments_ = std::move(items);
    nextId_ = 1;

    for (const auto& a : appointments_) {
        nextId_ = std::max(nextId_, a.id + 1);
    }
}

}  // namespace hms
