#pragma once
// Owner: Pranav Swaroop.  Includes: Status.h, Types.h
// Billing does not call other managers; the CLI checks that the patient exists before creating an invoice.
#include <optional>
#include <string>
#include <vector>

#include "hms/Status.h"
#include "hms/Types.h"

namespace hms {

class Billing {
public:
    Result<Invoice> createInvoice(Id patientId, Id appointmentId, long long amountCents,
                                  const std::string& description);   // amountCents must be > 0
    Status markPaid(Id invoiceId);

    std::optional<Invoice> findById(Id id) const;
    std::vector<Invoice> forPatient(Id patientId) const;
    long long outstandingCents(Id patientId) const;   // sum of Unpaid invoices

    const std::vector<Invoice>& all() const { return invoices_; }
    void replaceAll(std::vector<Invoice> items);

private:
    std::vector<Invoice> invoices_;
    Id nextId_ = 1;
};

}  // namespace hms
