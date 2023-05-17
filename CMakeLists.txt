#
#
#
cmake_minimum_required(VERSION 3.25)
project(osquery-extension-ping)

set(CMAKE_CXX_STANDARD 14)

find_package(GTest REQUIRED)
include_directories(
        /usr/local/include
        /users/eoche/osquery
        src
        ${GLOG_INCLUDE_DIRS}
        ${GFLAGS_INCLUDE_DIRS}
)

add_executable(osquery-extension-ping
        main.cpp
        src/table.cpp
        src/table.h
        src/shell.cpp
        src/shell.h
        tests/table_test.cpp
)

target_link_libraries(osquery-extension-ping ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
