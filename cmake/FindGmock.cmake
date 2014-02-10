# - Find 
# Find the native MySQL includes and library
#
#  GMOCK_INCLUDE_DIR - where to find tralib.h, etc.
#  GMOCK_LIBRARIES   - List of libraries when using Gmock
#  GMOCK_FOUND       - True if Gmock found.

UNSET(GMOCK_INCLUDE_DIR)
UNSET(GMOCK_LIBRARY_DIR)

FIND_PATH(GMOCK_INCLUDE_DIR gmock/gmock.h
    HINTS
    ${DEPENDENCY_ROOT}/com.google-gmock-static.tar.gz/include
    ${PROJECT_SOURCE_DIR}/ext/libgmock/include
    NO_DEFAULT_PATH
)

FIND_PATH(GMOCK_LIBRARY_DIR libgmock.a
    ${DEPENDENCY_ROOT}/com.google-gmock-static.tar.gz/lib
    ${PROJECT_SOURCE_DIR}/ext/libgmock/lib
    NO_DEFAULT_PATH
)

FIND_PATH(GMOCK_SCRIPTS_DIR generator/gmock_gen.py
    ${DEPENDENCY_ROOT}/com.google-gmock-static.tar.gz/scripts
    ${PROJECT_SOURCE_DIR}/ext/libgmock/scripts
    NO_DEFAULT_PATH
)

SET(GMOCK_NAMES gmock gmock_main)

SET(CMAKE_LIBRARY_PATH "")
FIND_LIBRARY(GMOCK_LIBRARY
             NAMES ${GMOCK_NAMES}
             PATHS ${DEPENDENCY_ROOT}/com.google-gmock-static.tar.gz/lib ${PROJECT_SOURCE_DIR}/ext/libgmock/lib
             NO_DEFAULT_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_PATH
             NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH
             NO_CMAKE_FIND_ROOT_PATH
            )

IF (GMOCK_INCLUDE_DIR AND GMOCK_LIBRARY)
    SET(GMOCK_FOUND TRUE)
    SET( GMOCK_LIBRARIES ${GMOCK_LIBRARY} )
ELSE (GMOCK_INCLUDE_DIR AND GMOCK_LIBRARY)
    SET(GMOCK_FOUND FALSE)
    SET( GMOCK_LIBRARIES )
ENDIF (GMOCK_INCLUDE_DIR AND GMOCK_LIBRARY)

IF (GMOCK_FOUND)
    IF (NOT GMOCK_FIND_QUIETLY)
        MESSAGE(STATUS "Found Gmock: ${GMOCK_LIBRARY}")
    ENDIF (NOT GMOCK_FIND_QUIETLY)
ELSE (GMOCK_FOUND)
    IF (GMOCK_FIND_REQUIRED)
        MESSAGE(STATUS "Looked for Gmock libraries named ${GMOCK_NAMES}.")
        MESSAGE(FATAL_ERROR "Could NOT find Gmock library")
    ENDIF (GMOCK_FIND_REQUIRED)
ENDIF (GMOCK_FOUND)

MARK_AS_ADVANCED(GMOCK_LIBRARY GMOCK_INCLUDE_DIR, GMOCK_LIBRARY_DIR, GMOCK_SCRIPTS_DIR)

