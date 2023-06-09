//
// Created by Emmanuel Oche on 5/17/23.
//

#pragma once

#include <osquery/sdk/sdk.h>
#include <osquery/core/system.h>
#include <osquery/sql/dynamic_table_row.h>
#include "shell.h"

namespace extension {
    /**
    * @brief PingTablePlugin is the main plugin that registers the table definition and generating the data needed to
    * populate the table when a query is executed against it
     *
     * It generates a table with two (2) columns latency and host
    */
    class PingTablePlugin final : public osquery::TablePlugin {
        // Set up the Test Class as Friend to allow unit testing the private methods of the plugin
        friend class PingTablePluginTest;
        FRIEND_TEST(PingTablePluginTest, TestIsValidHost);
        FRIEND_TEST(PingTablePluginTest, TestPingHost);
        FRIEND_TEST(PingTablePluginTest, TestParsePingOutputForLatencies);
        FRIEND_TEST(PingTablePluginTest, TestTableGenerate);
        /**
        * @brief is an interface for executing shell commands. This is added to enable the implementation
        * of command execution to be fluid.
        */
        Shell shell;
        /**
         * @brief sanitizes the provided host value to prevent sql injection and unauthorized access
         * to additional data. It validates if the input is a valid host or Ip Address
         * @param input the input to sanitize in this case the host name that was provided in the query
         * @return true if it is a valid host
         */
        virtual bool isValidHost(const std::string& input);
        /**
         * @brief executes a ping command on the shell for the given host
         * @param host is the host to ping
         * @return the output of the ping command which will be parsed to retrieve the latency
         */
        virtual std::string pingHost(std::string host);
        /**
         * @brief parses the result of the ping command to extract the latency
         * @param output is the output of the ping command sent for parsing
         * @return the list of latencies parsed from the ping command output
         */
        virtual std::vector<double> parsePingOutputForLatencies(std::string output);
        public:
            /**
             * @brief returns the list of columns that are supported by this plugin
             * @return the list of Osquery TableColumns
             */
            osquery::TableColumns columns() const override;
            /**
             * @brief This generates the actual data that is shown in the table when a query is executed against the
             * table in Osquery.
             * @param request a request sent to Osquery from the client
             * @return the list of Osquery TableRows containing the result of the query
             */
            osquery::TableRows generate(osquery::QueryContext& request) override;
            /**
             * The constructor for the Ping Table Plugin
             */
            PingTablePlugin();
            /**
             * The destructor for the Ping Table Plugin
             */
            ~PingTablePlugin() override;
    };
}

