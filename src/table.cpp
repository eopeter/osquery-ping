//
// Created by Emmanuel Oche on 5/17/23.
//

#include "table.h"
#include <osquery/logger/logger.h>
#include <osquery/sdk/sdk.h>
#include <string>
#include <utility>
#include <regex>
#include "shell.h"

using namespace extension;

const std::string kPingExecPath = "/sbin/ping";

PingTablePlugin::PingTablePlugin() {
    shell = Shell();
}

std::string PingTablePlugin::pingHost(std::string host) {
    ProcessOutput ping_output;
    std::vector<std::string> ping_args = {"-c", "1", std::move(host) };
    if (!shell.Execute(ping_output, kPingExecPath, ping_args) ||
        ping_output.exit_code != 0) {
        // VLOG(1) << "Failed to execute ping command";
        return "";
    }
    return ping_output.std_output;
}

std::vector<double> PingTablePlugin::parsePingOutputForLatencies(std::string output) {
    // output format: 64 bytes from 93.184.216.34: icmp_seq=0 ttl=57 time=11.303 ms
    std::regex timeRegex(R"time(time=([\d.]+) ms)time");

    std::sregex_iterator it(output.begin(), output.end(), timeRegex);
    std::sregex_iterator end;
    std::vector<double> latencies;
    while (it != end) {
        std::smatch match = *it;
        double timeMs = std::stod(match[1].str());
        latencies.emplace_back(timeMs);
        std::cout << "Round-trip time: " << timeMs << " ms" << std::endl;
        ++it;
    }
    return latencies;
}

osquery::TableColumns PingTablePlugin::columns() const {
    return {
        std::make_tuple("latency", osquery::INTEGER_TYPE, osquery::ColumnOptions::DEFAULT),
        std::make_tuple("host", osquery::TEXT_TYPE, osquery::ColumnOptions::DEFAULT)
    };
}

osquery::TableRows PingTablePlugin::generate(osquery::QueryContext& request) {
    osquery::TableRows results;

    // get the hosts from the request using a predicate
    auto hosts = request.constraints["host"].getAll(osquery::EQUALS);
    if (hosts.empty()) {
        // host is required to ping; so return
        return results;
    }

    // process all hosts
    for (const auto& host : hosts) {

        // ping the host
        std::string pingOutput = pingHost(host);
        if (pingOutput.empty()) {
            return results;
        }

        // parse the ping response
        auto latencies = parsePingOutputForLatencies(pingOutput);
        if (latencies.empty()) {
            return results;
        }

        // write the latencies to table rows
        for (const auto& latency : latencies) {
            osquery::DynamicTableRowHolder r;
            r["latency"] = std::to_string(latency);
            r["host"] = host;
            results.emplace_back(r);
        }
    }
    return results;
}

PingTablePlugin::~PingTablePlugin() {
    // tear down
}