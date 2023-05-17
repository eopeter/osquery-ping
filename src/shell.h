//
// Created by Emmanuel Oche on 5/17/23.
//
#pragma once

#include <string>
#include <vector>

struct ProcessOutput final {
    std::string std_output;
    std::string std_error;
    int exit_code;
};

namespace extension {
    class Shell {
        public:
            /**
             * @brief executes the given path and arguments and writes the result by reference to the output
             * @param output the output of the execution
             * @param path the path to the binary to execute
             * @param args the set of arguments passed to the binary for execution
             * @return if the execution succeeded
             */
            virtual bool Execute(ProcessOutput& output,
                                 const std::string& path,
                                 const std::vector<std::string>& args);

    };
}


