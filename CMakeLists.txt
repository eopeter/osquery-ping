#
#  Created by Emmanuel Oche on 5/17/23.
#
cmake_minimum_required(VERSION 3.25)
project(osquery-extension-ping)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)

function(main)

    # Add extension source files
    set(PROJECT_SOURCE_FILES

            src/table.cpp
            src/table.h

            src/shell.cpp
            src/shell.h

            tests/table_test.cpp
    )

    # Add the path to osquery
    set(OSQUERY_PATH $ENV{HOME}/osquery)

    # Add the osquery SDK directory
    set(OSQUERY_SDK_DIR ${OSQUERY_PATH}/build/osquery/sdk)

    # Add path to local includes for other libraries
    set(USR_LOCAL_INCLUDE_DIR /usr/local/include)

    # find packages needed to build
    find_package(GTest REQUIRED)
    find_package(Boost REQUIRED COMPONENTS thread system)

    # Specify the include directories for the osquery SDK and header files
    include_directories(
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${USR_LOCAL_INCLUDE_DIR}

            ${OSQUERY_PATH}
            ${OSQUERY_SDK_DIR}

            ${gflags_INCLUDE_DIR}
    )

    # Specify the link directories for the osquery library files
    link_directories(
            ${OSQUERY_PATH}
    )

    # Add the executable target for your extension and link against the osquery libraries
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

    addOsqueryExtensionEx("PingTablePlugin" "table" "ping"
        SOURCES ${PROJECT_SOURCE_FILES}
        INCLUDEDIRS "${CMAKE_CURRENT_SOURCE_DIR}/src"
        MAININCLUDES src/table.h
        LIBRARIES ${project_libraries}
    )

endfunction()

main()