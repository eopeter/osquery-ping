//
// Created by Emmanuel Oche on 5/17/23.
//

#pragma once

#include <osquery/sdk/sdk.h>
#include <osquery/core/system.h>
#include <osquery/sql/dynamic_table_row.h>
#include "shell.h"

namespace extension {
    class PingTablePlugin final : public osquery::TablePlugin {
        public:
            Shell shell;
            osquery::TableColumns columns() const override;
            osquery::TableRows generate(osquery::QueryContext& request) override;
            virtual std::string pingHost(std::string host);
            virtual std::vector<double> parsePingOutputForLatencies(std::string output);
            PingTablePlugin();
            ~PingTablePlugin() override;
    };

    // define the ping table.
    REGISTER_EXTERNAL(PingTablePlugin, "table", "ping");
}

