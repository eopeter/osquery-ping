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
        virtual bool Execute(ProcessOutput& output,
                             const std::string& path,
                             const std::vector<std::string>& args);

    };
}


