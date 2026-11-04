#include "hms/Billing.h"

#include <algorithm>
#include <utility>

namespace hms {

Result<Invoice> Billing::createInvoice(Id patientId, Id appointmentId, long long amountCents,
                                      const std::string& description) {
    if (patientId <= 0) {
        return Result<Invoice>::failure("Invalid patient ID.");
    }
    if (amountCents <= 0) {
        return Result<Invoice>::failure("Invoice amount must be greater than zero.");
    }

    Invoice invoice;
    invoice.id = nextId_++;
    invoice.patientId = patientId;
    invoice.appointmentId = appointmentId;
    invoice.amountCents = amountCents;
    invoice.description = description;
    invoice.status = InvoiceStatus::Unpaid;

    invoices_.push_back(invoice);
    return Result<Invoice>::success(invoice);
}

Status Billing::markPaid(Id invoiceId) {
    const auto it = std::find_if(invoices_.begin(), invoices_.end(), [invoiceId](const Invoice& inv) {
        return inv.id == invoiceId;
    });

    if (it == invoices_.end()) {
        return Status::failure("Invoice not found.");
    }

    if (it->status == InvoiceStatus::Paid) {
        return Status::failure("Invoice is already paid.");
    }

    it->status = InvoiceStatus::Paid;
    return Status::success();
}

std::optional<Invoice> Billing::findById(Id id) const {
    const auto it = std::find_if(invoices_.begin(), invoices_.end(), [id](const Invoice& inv) {
        return inv.id == id;
    });

    if (it == invoices_.end()) {
        return std::nullopt;
    }
    return *it;
}

std::vector<Invoice> Billing::forPatient(Id patientId) const {
    std::vector<Invoice> result;
    for (const auto& inv : invoices_) {
        if (inv.patientId == patientId) {
            result.push_back(inv);
        }
    }
    return result;
}

long long Billing::outstandingCents(Id patientId) const {
    long long total = 0;
    for (const auto& inv : invoices_) {
        if (inv.patientId == patientId && inv.status == InvoiceStatus::Unpaid) {
            total += inv.amountCents;
        }
    }
    return total;
}

void Billing::replaceAll(std::vector<Invoice> items) {
    invoices_ = std::move(items);
    nextId_ = 1;
    for (const auto& i : invoices_) {
        nextId_ = std::max(nextId_, i.id + 1);
    }
}

}  // namespace hms
