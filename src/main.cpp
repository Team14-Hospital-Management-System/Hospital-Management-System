#include <iostream>
#include <string>

#include "hms/AppContext.h"
#include "hms/Cli.h"

int main(int argc, char** argv) {
    const std::string dataDir = (argc > 1) ? argv[1] : "data";
    hms::AppContext ctx(dataDir);
    return hms::runCli(ctx, std::cin, std::cout);
}
