#include <istream>
#include <ostream>

#include "hms/AppContext.h"
#include "hms/Cli.h"

namespace hms {

// TODO(Pranav Swaroop): implement the Billing menu using ctx.billing.
void runBillingMenu(AppContext&, std::istream&, std::ostream& out) {
    out << "Billing menu: not implemented yet.\n";
}

}  // namespace hms
