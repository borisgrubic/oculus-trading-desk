
# - Enable Code Coverage
#
#
# USAGE:
# 1. Copy this file into your cmake modules path
# 2. Add the following line to your CMakeLists.txt:
#      include(UseCodeCoverage)
# 3. Select the ENABLE_CODECOVERAGE option when you want to build with code coverage enabled.
# 
# Variables you may define are:
#  CODECOV_OUTPUTFILE - the name of the temporary output file used. Defaults to "cmake_coverage.output"
#  CODECOV_HTMLOUTPUTDIR - the name of the directory where HTML results are placed. Defaults to "coverage_results"
#

#
#  Copyright (C) 2010 Brad Hards <bradh at frogmouth.net>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

OPTION( ENABLE_CODECOVERAGE "Enable code coverage testing support" )

if ( ENABLE_CODECOVERAGE )

    if ( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" )
        message( WARNING "Code coverage results with an optimised (non-Debug) build may be misleading" )
    endif ( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" )

    if ( NOT DEFINED CODECOV_OUTPUTFILE )
        set( CODECOV_OUTPUTFILE cmake_coverage.output )
    endif ( NOT DEFINED CODECOV_OUTPUTFILE )

    if ( NOT DEFINED CODECOV_HTMLOUTPUTDIR )
        set( CODECOV_HTMLOUTPUTDIR coverage_results )
    endif ( NOT DEFINED CODECOV_HTMLOUTPUTDIR )

    if ( CMAKE_COMPILER_IS_GNUCXX )
        find_program( CODECOV_GCOV gcov )
        find_program( CODECOV_LCOV lcov )
        find_program( CODECOV_GENHTML genhtml )

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage ")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage ")
        link_libraries( gcov )

        set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage ")

        # for lcov-1.10 , we will need to add this to the command: -rc lcov_branch_coverage=1

        # We need to initialize the coverage information upon creating the
        # library
        add_custom_command(TARGET src POST_BUILD
            COMMAND ${CODECOV_LCOV}  -b . -d ${CMAKE_BINARY_DIR} -o ${CODECOV_OUTPUTFILE} -c -i)

        # add a coverage target 
        add_custom_target(check
            # Run tests
            COMMAND ${CMAKE_CTEST_COMMAND} ARGS=-j4
            # Init Coverage Data
            COMMAND ${CODECOV_LCOV} -b . -d ${CMAKE_BINARY_DIR} -o ${CODECOV_OUTPUTFILE} -c 
            # Clean Coverage Data
            COMMAND ${CODECOV_LCOV} -e ${CODECOV_OUTPUTFILE} '${PROJECT_SOURCE_DIR}/src*' -o   ${CODECOV_OUTPUTFILE}
            # Generate Report
            COMMAND ${CODECOV_GENHTML} -o ${CODECOV_HTMLOUTPUTDIR}  ${CODECOV_OUTPUTFILE} 
        )

    endif ( CMAKE_COMPILER_IS_GNUCXX )

endif (ENABLE_CODECOVERAGE )

