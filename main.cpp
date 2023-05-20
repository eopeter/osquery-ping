//
// Created by Emmanuel Oche on 5/17/23.
//

#include <osquery/sdk/sdk.h>
#include <osquery/core/system.h>
#include <osquery/logger/logger.h>
#include "src/table.h"

using namespace extension;

// define the ping table.
REGISTER_EXTERNAL(PingTablePlugin, "table", "ping");

int main(int argc, char* argv[]) {
    // Start logging, threads, etc.
    osquery::Initializer runner(argc, argv, osquery::ToolType::EXTENSION);

    // Connect to osqueryi or osqueryd.
    auto status = osquery::startExtension("ping", "0.0.1");
    if (!status.ok()) {
        LOG(ERROR) << status.getMessage();
        runner.requestShutdown(status.getCode());
    }

    // Finally wait for a signal / interrupt to shut down.
    runner.waitForShutdown();
    return runner.shutdown(0);
}
