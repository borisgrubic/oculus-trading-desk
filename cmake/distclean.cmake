
set(cmake_generated CMakeCache.txt
                    Makefile
                    CTestTestfile.cmake
                    cmake_install.cmake
                    CPackConfig.cmake
                    CPackSourceConfig.cmake
                    DartConfiguration.tcl
                    CMakeCache.txt
                    CMakeFiles
                    Testing
                    cmake_coverage.output
                    testResults*.xml
                    *.gcno
                    *.gcda

)

execute_process(COMMAND make clean ERROR_QUIET)
foreach(file ${cmake_generated})
    execute_process(COMMAND find ${CMAKE_BINARY_DIR} -name ${file} -exec rm -r {} \; 
        ERROR_QUIET)
endforeach(file)
