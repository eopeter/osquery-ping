//
// Created by Emmanuel Oche on 5/17/23.
//

#include "table.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <osquery/sdk/sdk.h>
#include <string>

using ::testing::AtLeast;

namespace table_tests {

    class MockShell : public extension::Shell {
        public:
            MOCK_METHOD0(Execute, void());
    };

    class PingTablePluginTest : public testing::Test {
    protected:
        void SetUp() override {

        }
        void TearDown() override {

        }
    };

    TEST_F(PingTablePluginTest, TestParsePingOutputForLatencies) {
        struct test {
            std::string pingCommand;
            std::string output;
            int expectedLatencyCount{};
            std::vector<double> expectedLatencies;
        };
        test singlePing, multiplePing, badHost, noHost;

        singlePing.pingCommand = "ping -c 1 abc.com";
        singlePing.expectedLatencyCount = 1;
        singlePing.expectedLatencies = { 35.532 };
        singlePing.output = "PING abc.com (142.251.40.206): 56 data bytes\n"
                        "64 bytes from 142.251.40.206: icmp_seq=0 ttl=117 time=35.532 ms\n"
                        "\n"
                        "--- abc.com ping statistics ---\n"
                        "1 packets transmitted, 1 packets received, 0.0% packet loss\n"
                        "round-trip min/avg/max/stddev = 35.532/35.532/35.532/0.000 ms";

        multiplePing.pingCommand = "ping -c 4 abc.com";
        multiplePing.expectedLatencyCount = 4;
        multiplePing.expectedLatencies = { 39.901, 32.460, 32.881, 36.064 };
        multiplePing.output = "PING abc.com (142.250.80.78): 56 data bytes\n"
                          "64 bytes from 142.250.80.78: icmp_seq=0 ttl=58 time=39.901 ms\n"
                          "64 bytes from 142.250.80.78: icmp_seq=1 ttl=58 time=32.460 ms\n"
                          "64 bytes from 142.250.80.78: icmp_seq=2 ttl=58 time=32.881 ms\n"
                          "64 bytes from 142.250.80.78: icmp_seq=3 ttl=58 time=36.064 ms\n"
                          "\n"
                          "--- abc.com ping statistics ---\n"
                          "4 packets transmitted, 4 packets received, 0.0% packet loss\n"
                          "round-trip min/avg/max/stddev = 32.460/35.327/39.901/2.986 ms";

        badHost.pingCommand = "ping -c 1 abc";
        badHost.expectedLatencyCount = 0;
        badHost.expectedLatencies = {};
        badHost.output = "ping: cannot resolve abc.: Unknown host";

        noHost.pingCommand = "ping -c 1";
        noHost.expectedLatencyCount = 0;
        noHost.expectedLatencies = {};

        std::vector<test> testCases = {singlePing, multiplePing, badHost, noHost};
        extension::PingTablePlugin table;
       for(const auto& testCase : testCases) {
           std::vector<double> latencies = table.parsePingOutputForLatencies(testCase.output);

           // Check the number of latencies returned
           ASSERT_EQ(latencies.size(), testCase.expectedLatencyCount);

           // Check that the correct latency value in ms was returned
           for(int i = 0; i < latencies.size(); i++) {
               ASSERT_EQ(latencies[i], testCase.expectedLatencies[i]);
           }
       }
    }

    TEST_F(PingTablePluginTest, TestPingHost) {
        MockShell shell;

        EXPECT_CALL(shell, Execute())              // #3
                .Times(AtLeast(1));

        extension::PingTablePlugin table;
        table.shell = shell;

        std::string output = table.pingHost("abc.com");
    }

    TEST_F(PingTablePluginTest, TestTableGenerate) {
        extension::PingTablePlugin table;

        // Set the process events in the query context
        osquery::QueryContext ctx;

        // Generate the query data
        auto results = table.generate(ctx);

        // Check that no results are returned
        ASSERT_EQ(results.size(), 0);
    }

}

int main(int argc, char** argv) {
    // The following line must be executed to initialize Google Mock
    // (and Google Test) before running the tests.
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}