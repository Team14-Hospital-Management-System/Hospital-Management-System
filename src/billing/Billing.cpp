#include "hms/Billing.h"

#include <algorithm>

namespace hms {

// TODO(Pranav Swaroop): implement invoices and payment status.
Result<Invoice> Billing::createInvoice(Id, Id, long long, const std::string&) { return Result<Invoice>::failure("not implemented yet"); }
Status Billing::markPaid(Id) { return Status::failure("not implemented yet"); }
std::optional<Invoice> Billing::findById(Id) const { return std::nullopt; }
std::vector<Invoice> Billing::forPatient(Id) const { return {}; }
long long Billing::outstandingCents(Id) const { return 0; }

void Billing::replaceAll(std::vector<Invoice> items) {
    invoices_ = std::move(items);
    nextId_ = 1;
    for (const auto& i : invoices_) nextId_ = std::max(nextId_, i.id + 1);
}

}  // namespace hms
