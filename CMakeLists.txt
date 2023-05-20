#
#  Created by Emmanuel Oche on 5/17/23.
#
project("osquery-extension-ping")

function(main)

    # Add extension source files
    set(PROJECT_SOURCE_FILES

            main.cpp

            src/table.cpp
            src/table.h

            src/shell.cpp
            src/shell.h

            )

    # Specify the include directories for the osquery SDK and header files
    include_directories(
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    addOsqueryExtension(
            "${PROJECT_NAME}"
            ${PROJECT_SOURCE_FILES}
    )

endfunction()

main()