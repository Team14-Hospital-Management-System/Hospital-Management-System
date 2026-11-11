#include "TestHelper.h"
#include "TestData.h"
#include "hms/Billing.h"

int main() {
    hms::Billing b;
    CHECK(b.all().empty());

    // 1. Create first invoice
    auto inv1 = b.createInvoice(1, 10, 5000, "Consultation fee");
    CHECK(inv1.ok());
    CHECK(inv1.value.id == 1);
    CHECK(inv1.value.patientId == 1);
    CHECK(inv1.value.appointmentId == 10);
    CHECK(inv1.value.amountCents == 5000);
    CHECK(inv1.value.description == "Consultation fee");
    CHECK(inv1.value.status == hms::InvoiceStatus::Unpaid);
    CHECK(b.all().size() == 1);

    // 2. Create second invoice (without appointment)
    auto inv2 = b.createInvoice(1, hms::kInvalidId, 2500, "Laboratory tests");
    CHECK(inv2.ok());
    CHECK(inv2.value.id == 2);
    CHECK(inv2.value.appointmentId == hms::kInvalidId);
    CHECK(b.all().size() == 2);

    // 3. Validation: amounts <= 0 and invalid patients must fail
    auto zeroAmt = b.createInvoice(1, 10, 0, "Zero amount");
    CHECK(!zeroAmt.ok());
    auto negAmt = b.createInvoice(1, 10, -100, "Negative amount");
    CHECK(!negAmt.ok());
    auto invalidPatient = b.createInvoice(hms::kInvalidId, 10, 1000, "Invalid patient");
    CHECK(!invalidPatient.ok());

    // 4. findById
    auto found = b.findById(1);
    CHECK(found.has_value());
    CHECK(found->description == "Consultation fee");
    auto notFound = b.findById(999);
    CHECK(!notFound.has_value());

    // 5. forPatient
    auto p1Invoices = b.forPatient(1);
    CHECK(p1Invoices.size() == 2);
    auto p2Invoices = b.forPatient(2);
    CHECK(p2Invoices.empty());

    // 6. outstandingCents & unpaid queries before payment
    CHECK(b.outstandingCents(1) == 7500);
    CHECK(b.outstandingCents(2) == 0);
    CHECK(b.unpaidForPatient(1).size() == 2);
    CHECK(b.paidForPatient(1).empty());
    CHECK(b.totalRevenueCents() == 0);

    // 7. markPaid
    auto payStatus = b.markPaid(1);
    CHECK(payStatus.ok);
    CHECK(b.findById(1)->status == hms::InvoiceStatus::Paid);
    CHECK(b.outstandingCents(1) == 2500);
    CHECK(b.unpaidForPatient(1).size() == 1);
    CHECK(b.paidForPatient(1).size() == 1);
    CHECK(b.totalRevenueCents() == 5000);

    // 8. markPaid on already paid invoice fails
    auto payAgain = b.markPaid(1);
    CHECK(!payAgain.ok);

    // 9. markPaid on non-existent invoice fails
    auto payNonExistent = b.markPaid(999);
    CHECK(!payNonExistent.ok);

    // 10. markPaid on second invoice
    CHECK(b.markPaid(2).ok);
    CHECK(b.outstandingCents(1) == 0);
    CHECK(b.unpaidForPatient(1).empty());
    CHECK(b.paidForPatient(1).size() == 2);
    CHECK(b.totalRevenueCents() == 7500);

    // 11. replaceAll and nextId continuity
    hms::Billing restored;
    restored.replaceAll(b.all());
    CHECK(restored.all().size() == 2);
    CHECK(restored.findById(1)->status == hms::InvoiceStatus::Paid);
    CHECK(restored.outstandingCents(1) == 0);
    CHECK(restored.totalRevenueCents() == 7500);

    auto nextInv = restored.createInvoice(2, hms::kInvalidId, 1200, "Pharmacy");
    CHECK(nextInv.ok());
    CHECK(nextInv.value.id == 3);
    CHECK(restored.outstandingCents(2) == 1200);
    CHECK(restored.unpaidForPatient(2).size() == 1);
    CHECK(restored.totalRevenueCents() == 7500);

    return TEST_RESULT();
}
