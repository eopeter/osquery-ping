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
    // initialize the shell
    shell = Shell();
}

osquery::TableColumns PingTablePlugin::columns() const {
    // returns tuples that defines each column in the table with their name and data type
    return {
        std::make_tuple("latency", osquery::INTEGER_TYPE, osquery::ColumnOptions::DEFAULT),
        std::make_tuple("host", osquery::TEXT_TYPE, osquery::ColumnOptions::DEFAULT)
    };
}

osquery::TableRows PingTablePlugin::generate(osquery::QueryContext& request) {
    // init the results rows that will be returned
    osquery::TableRows results;

    // get the hosts from the request using a predicate. This will match `WHERE host = '127.0.0.1' of the query.
    auto hosts = request.constraints["host"].getAll(osquery::EQUALS);
    if (hosts.empty()) {
        // host is required to ping, return an empty table
        return results;
    }

    // process all hosts found using the predicate
    for (const auto& host : hosts) {

        // only valid hosts will be used
        if (!isValidHost(host)) continue;

        // ping the host
        std::string pingOutput = pingHost(host);
        if (pingOutput.empty()) {
            // if no results to ping, return an empty table
            return results;
        }

        // parse the ping response
        auto latencies = parsePingOutputForLatencies(pingOutput);
        if (latencies.empty()) {
            // if the parser does not find any latencies, return an empty table
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

bool PingTablePlugin::isValidHost(const std::string& input) {
    // TODO: implement sanization of input to ensure it meets the definition of the expected input

    // regex to check that it is a valid domain
    std::regex domainRegex(R"(^(?:[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?\.)+(?:[a-zA-Z]{2,})$)");
    auto isValidDomain = std::regex_match(input, domainRegex);

    // regex  to check that is is a valid IPAddress (IPv4 or IPv6)
    std::regex ipv4Regex(R"(^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)");
    auto isValidIpv4 = std::regex_match(input, ipv4Regex);

    std::regex ipv6Regex(R"(^([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}$)");
    auto isValidIpv6 = std::regex_match(input, ipv6Regex);

    return isValidDomain || isValidIpv4 || isValidIpv6;
}

std::string PingTablePlugin::pingHost(std::string host) {
    // holds the output of the ping result
    ProcessOutput ping_output;

    // the list of arguments passed to the ping command
    std::vector<std::string> ping_args = {"-c", "1", std::move(host) };

    // execute the ping command against the provided shell
    if (!shell.Execute(ping_output, kPingExecPath, ping_args) ||
        ping_output.exit_code != 0) {
        // log the failure of the ping command
        LOG(ERROR) << "Failed to execute ping command";
        return "";
    }
    // returns the response of the ping command which is held in standard out
    return ping_output.std_output;
}

std::vector<double> PingTablePlugin::parsePingOutputForLatencies(std::string output) {

    // Regex to parse the output format: 64 bytes from 93.184.216.34: icmp_seq=0 ttl=57 time=11.303 ms
    std::regex timeRegex(R"time(time=([\d.]+) ms)time");

    // iterator to find matches for the latency based on the regex
    std::sregex_iterator it(output.begin(), output.end(), timeRegex);
    std::sregex_iterator end;
    // the latencies will be captured as doubles to allow sorting
    std::vector<double> latencies;
    while (it != end) {
        std::smatch match = *it;
        // parse the match to a double
        double timeMs = std::stod(match[1].str());
        latencies.emplace_back(timeMs);
        // log an info level
        LOG(INFO) << "Round-trip time: " << timeMs << " ms" << std::endl;
        ++it;
    }
    return latencies;
}

PingTablePlugin::~PingTablePlugin() = default;