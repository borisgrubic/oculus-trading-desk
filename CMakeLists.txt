PROJECT(VRTradingDesk)
cmake_minimum_required (VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(EXT_ROOT ${PROJECT_SOURCE_DIR}/ext)

include(CTest)
include(Version)
include(Executable)
include(Library)
include(Test)
include(Package)

SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE OFF)
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()


find_package (Gtest REQUIRED)
find_package (Gmock REQUIRED)
find_package (Boost REQUIRED)

include_directories ("${PROJECT_SOURCE_DIR}")

include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/ext/LibOVR/Include/")
include_directories ("/usr/include/GL")
link_directories("${PROJECT_SOURCE_DIR}/ext/LibOVR/Lib/Linux/Release/x86_64/")

add_definitions(-DHAVE_CONFIG_H)
add_definitions(-D_GNU_SOURCE)

set(DEFAULT_WARNINGS "-Wall -Wextra -Werror -Wno-sign-compare -Wno-unused-result -Wno-unused-parameter -Wno-error=type-limits -Wno-error=unused-but-set-variable -Wno-inline -fno-strict-aliasing -Wno-unused-local-typedefs -Wno-unused-value")

set(CMAKE_C_FLAGS_DEBUG     "${DEFAULT_WARNINGS} -std=gnu11 -O0 -ggdb ")
set(CMAKE_CXX_FLAGS_DEBUG   "${DEFAULT_WARNINGS} -std=c++11 -O0 -ggdb ")
set(CMAKE_C_FLAGS_RELEASE   "${DEFAULT_WARNINGS} -std=gnu11 -O3 -ggdb ")
set(CMAKE_CXX_FLAGS_RELEASE "${DEFAULT_WARNINGS} -std=c++11 -O3 -ggdb ")
set(CMAKE_LD_FLAGS "-static")

add_subdirectory (src) 
add_subdirectory (bin) 
add_subdirectory (test) 

add_custom_target(distclean COMMAND ${CMAKE_COMMAND} -P ${PROJECT_SOURCE_DIR}/cmake/distclean.cmake)

