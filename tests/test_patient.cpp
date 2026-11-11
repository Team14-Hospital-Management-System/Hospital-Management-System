#include "TestHelper.h"
#include "TestData.h"
#include "hms/PatientManager.h"

int main() {
    hms::PatientManager pm;
    CHECK(pm.all().empty());

    auto first = pm.registerPatient(testdata::patientOne());
    CHECK(first.ok());
    CHECK(first.value.id == 1);
    CHECK(pm.exists(1));
    CHECK(pm.findById(1).has_value());
    CHECK(pm.findById(1)->name == "Test Patient One");

    auto second = pm.registerPatient(testdata::patientTwo());
    CHECK(second.ok());
    CHECK(second.value.id == 2);
    CHECK(pm.all().size() == 2);

    CHECK(pm.searchByName("patient").size() == 2);
    CHECK(pm.searchByName("TEST PATIENT ONE").size() == 1);
    CHECK(pm.searchByName("missing").empty());

    auto updated = *pm.findById(1);
    updated.phone = "555-0199";
    updated.medicalNotes = "Updated fictional note";
    CHECK(pm.updatePatient(updated).ok);
    CHECK(pm.findById(1)->phone == "555-0199");
    CHECK(!pm.updatePatient(testdata::patientOne()).ok);
    updated.id = 999;
    CHECK(!pm.updatePatient(updated).ok);

    hms::Patient invalid;
    CHECK(!pm.registerPatient(invalid).ok());
    invalid.name = "Invalid Age";
    invalid.age = 131;
    CHECK(!pm.registerPatient(invalid).ok());

    hms::PatientManager restored;
    restored.replaceAll(pm.all());
    auto third = restored.registerPatient(testdata::patientOne());
    CHECK(third.ok());
    CHECK(third.value.id == 3);
    CHECK(!restored.exists(999));
    return TEST_RESULT();
}
