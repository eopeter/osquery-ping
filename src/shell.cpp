//
// Created by Emmanuel Oche on 5/17/23.
//

#include "shell.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>


bool extension::Shell::Execute(ProcessOutput& output,
                    const std::string& path,
                    const std::vector<std::string>& args) {
    output = {};

    try {
        // construct the command from the path and args
        std::string command = path;
        for (const auto& arg : args) {
            command += " " + arg;
        }

        // Open pipes for reading output and error
        FILE* pipe = popen(command.c_str(), "r");
        if (pipe == nullptr) {
            throw std::runtime_error("popen() failed!");
        }

        constexpr int bufferSize = 256;
        char buffer[bufferSize];

        std::string stdout_output;
        std::string stderr_output;

        // Read output from stdout pipe
        while (fgets(buffer, bufferSize, pipe) != nullptr) {
            stdout_output += buffer;
        }

        // Close the pipe and retrieve the exit code
        int exit_status = pclose(pipe);

        // Store the output, error, and exit code
        output.std_output = stdout_output;
        output.std_error = stderr_output + " command: " + command;
        output.exit_code = WEXITSTATUS(exit_status);

        return true;
    } catch (...) {
        return false;
    }
}

extension::Shell::~Shell() {

}
