//
// Created by Emmanuel Oche on 5/17/23.
//

#include "shell.h"
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <future>

namespace boostproc = boost::process;
namespace boostasio = boost::asio;

bool extension::Shell::Execute(ProcessOutput& output,
                    const std::string& path,
                    const std::vector<std::string>& args) {
    output = {};

    try {
        std::future<std::string> process_stdout;
        std::future<std::string> process_stderr;

        boostasio::io_service io_service;

        // clang-format off
        boostproc::child process(
                path, boostproc::args(args),
                boostproc::std_out > process_stdout,
                boostproc::std_err > process_stderr,
                io_service
        );
        // clang-format on

        io_service.run();

        output.std_output = process_stdout.get();
        output.std_error = process_stderr.get();
        output.exit_code = process.exit_code();

        return true;

    } catch (...) {
        return false;
    }
}
