
#include "TestHelper.h"
#include "TestData.h"

#include "hms/AppointmentManager.h"
#include "hms/DoctorManager.h"
#include "hms/PatientManager.h"

// test_appointment
// Owner: OP Medha
int main() {
    using namespace hms;

    PatientManager patients;
    DoctorManager doctors;
    AppointmentManager appointments(patients, doctors);

    CHECK(appointments.all().empty());

    // Create fictional test records.
    auto patientResult = patients.registerPatient(
        testdata::patientOne());

    auto doctorResult = doctors.addDoctor(
        testdata::doctorOne());

    CHECK(patientResult.ok());
    CHECK(doctorResult.ok());

    // Successful booking tests require implemented Patient and Doctor modules.
    if (patientResult.ok() && doctorResult.ok()) {
        const Id patientId = patientResult.value.id;
        const Id doctorId = doctorResult.value.id;

        // 1. Book an appointment.
        auto booking = appointments.book(
            patientId,
            doctorId,
            testdata::slot10am(),
            "General checkup");

        CHECK(booking.ok());
        CHECK(appointments.all().size() == 1);

        if (booking.ok()) {
            const Id appointmentId = booking.value.id;

            CHECK(booking.value.status == AppointmentStatus::Scheduled);
            CHECK(!appointments.isSlotFree(
                doctorId, testdata::slot10am()));

            // 2. Prevent double booking.
            auto duplicate = appointments.book(
                patientId,
                doctorId,
                testdata::slot10am(),
                "Duplicate appointment");

            CHECK(!duplicate.ok());

            // 3. Find appointment by ID.
            auto found = appointments.findById(appointmentId);
            CHECK(found.has_value());

            // 4. Reschedule to a free slot.
            const TimeSlot slot12{"2030-01-15", "12:00"};

            Status rescheduleResult =
                appointments.reschedule(appointmentId, slot12);

            CHECK(rescheduleResult.ok);
            CHECK(appointments.isSlotFree(
                doctorId, testdata::slot10am()));
            CHECK(!appointments.isSlotFree(doctorId, slot12));

            // 5. Check filters.
            CHECK(appointments.forPatient(patientId).size() == 1);
            CHECK(appointments.forDoctor(doctorId).size() == 1);
            CHECK(appointments.onDate("2030-01-15").size() == 1);

            // 6. Cancel appointment.
            Status cancelResult = appointments.cancel(appointmentId);

            CHECK(cancelResult.ok);
            CHECK(appointments.isSlotFree(doctorId, slot12));

            // Cannot cancel it twice.
            CHECK(!appointments.cancel(appointmentId).ok);

            // 7. Book the same slot after cancellation.
            auto secondBooking = appointments.book(
                patientId,
                doctorId,
                slot12,
                "Follow-up appointment");

            CHECK(secondBooking.ok());

            // 8. Mark the second appointment completed.
            if (secondBooking.ok()) {
                const Id secondId = secondBooking.value.id;

                CHECK(appointments.markCompleted(secondId).ok);

                auto completed = appointments.findById(secondId);
                CHECK(completed.has_value());

                if (completed.has_value()) {
                    CHECK(completed->status ==
                          AppointmentStatus::Completed);
                }

                // Completed appointments cannot be cancelled or rescheduled.
                CHECK(!appointments.cancel(secondId).ok);
                CHECK(!appointments.reschedule(
                    secondId, testdata::slot10am()).ok);
            }
        }
    }

    // Invalid IDs and nonexistent appointments.
    CHECK(!appointments.book(
        9999, 9999, testdata::slot10am(), "Invalid booking").ok());

    CHECK(!appointments.cancel(9999).ok);

    CHECK(!appointments.reschedule(
        9999, testdata::slot11am()).ok);

    return TEST_RESULT();
}
