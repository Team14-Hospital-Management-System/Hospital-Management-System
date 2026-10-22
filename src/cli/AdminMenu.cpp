#include <istream>
#include <ostream>

#include "hms/AppContext.h"
#include "hms/Cli.h"

namespace hms {

// TODO(Pranav Swaroop): implement the Administration menu using ctx.auth.
void runAdminMenu(AppContext&, std::istream&, std::ostream& out) {
    out << "Administration menu: not implemented yet.\n";
}

}  // namespace hms
