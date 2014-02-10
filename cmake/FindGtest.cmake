# - Find 
# Find the native MySQL includes and library
#
#  GTEST_INCLUDE_DIR - where to find tralib.h, etc.
#  GTEST_LIBRARIES   - List of libraries when using Gtest
#  GTEST_FOUND       - True if Gtest found.

FIND_PATH(GTEST_INCLUDE_DIR gtest/gtest.h
    ${DEPENDENCY_ROOT}/com.google-gtest-static.tar.gz/include
    ${PROJECT_SOURCE_DIR}/ext/libgtest/include
    NO_DEFAULT_PATH
)

FIND_PATH(GTEST_LIBRARY_DIR libgtest.so
    ${DEPENDENCY_ROOT}/com.google-gtest-static.tar.gz/lib
    ${PROJECT_SOURCE_DIR}/ext/libgtest/lib
    NO_DEFAULT_PATH
)

SET(GTEST_NAMES gtest gtest_main)

SET(CMAKE_LIBRARY_PATH "")
FIND_LIBRARY(GTEST_LIBRARY
             NAMES ${GTEST_NAMES}
             PATHS ${DEPENDENCY_ROOT}/com.google-gtest-static.tar.gz/lib ${PROJECT_SOURCE_DIR}/ext/libgtest/lib
             NO_DEFAULT_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_PATH
             NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH
             NO_CMAKE_FIND_ROOT_PATH
            )

IF (GTEST_INCLUDE_DIR AND GTEST_LIBRARY)
    SET(GTEST_FOUND TRUE)
    SET( GTEST_LIBRARIES ${GTEST_LIBRARY} )
ELSE (GTEST_INCLUDE_DIR AND GTEST_LIBRARY)
    SET(GTEST_FOUND FALSE)
    SET( GTEST_LIBRARIES )
ENDIF (GTEST_INCLUDE_DIR AND GTEST_LIBRARY)

IF (GTEST_FOUND)
    IF (NOT GTEST_FIND_QUIETLY)
        MESSAGE(STATUS "Found Gtest: ${GTEST_LIBRARY}")
    ENDIF (NOT GTEST_FIND_QUIETLY)
ELSE (GTEST_FOUND)
    IF (GTEST_FIND_REQUIRED)
        MESSAGE(STATUS "Looked for Gtest libraries named ${GTEST_NAMES}.")
        MESSAGE(FATAL_ERROR "Could NOT find Gtest library")
    ENDIF (GTEST_FIND_REQUIRED)
ENDIF (GTEST_FOUND)

MARK_AS_ADVANCED(GTEST_LIBRARY GTEST_INCLUDE_DIRi GTEST_LIBRARY_DIR)

