
MACRO(ADD_PROJECT_LIBRARY LIBRARY_NAME)
    FILE(GLOB LIBRARY_SOURCES *.cpp */*.cpp */*/*.cpp *.c */*.c */*/*.c)
    FILE(GLOB LIBRARY_HEADERS *.hpp */*.hpp */*/*.hpp *.h */*.h */*/*.h)

    if(BUILD_SHARED_LIBRARIES)
        add_library(${LIBRARY_NAME} SHARED ${LIBRARY_SOURCES})
        INSTALL(TARGETS ${LIBRARY_NAME} DESTINATION lib)

        foreach (HEADER ${LIBRARY_HEADERS})
            STRING(REGEX REPLACE "${PROJECT_SOURCE_DIR}/src/" "" HEADER_FILE "${HEADER}")
            STRING(REGEX REPLACE "(.*)/.*" "\\1" HEADER_PATH "${HEADER_FILE}")
            INSTALL(FILES ${HEADER} DESTINATION include/Colibri/${HEADER_PATH})
        endforeach (HEADER)
    else(BUILD_SHARED_LIBRARIES)
        add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})
    endif()


ENDMACRO(ADD_PROJECT_LIBRARY)
