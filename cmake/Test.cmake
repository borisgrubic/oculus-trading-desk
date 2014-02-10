MACRO(ADD_PROJECT_TEST TEST_NAME)
    add_executable(${TEST_NAME}er ${TEST_NAME}er.cpp)
    include_directories ("${GTEST_INCLUDE_DIR}")
    include_directories ("${GMOCK_INCLUDE_DIR}")

    target_link_libraries(${TEST_NAME}er ${GMOCK_LIBRARY} ${GTEST_LIBRARY} ${BOOST_SYSTEM_LIB} ${BOOST_THREAD_LIB} gcov pthread)
    foreach (value ${ARGN})
        if (NOT ${value} STREQUAL ${TEST_NAME})
            target_link_libraries(${TEST_NAME}er ${value})
        endif (NOT ${value} STREQUAL ${TEST_NAME})
    endforeach (value)


    set(CTEST_CUSTOM_PRE_TEST  "lcov -c --directory ${CMAKE_CURRENT_SOURCE_DIR} -i -o  initial.info")
    set(CTEST_CUSTOM_POST_TEST "lcov -c --directory ${CMAKE_CURRENT_SOURCE_DIR} -f -c -o final.info")
    add_test(NAME ${TEST_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${TEST_NAME}er --gtest_output=xml:${CMAKE_CURRENT_SOURCE_DIR}/testResults${TEST_NAME}.xml)
ENDMACRO(ADD_PROJECT_TEST) 
