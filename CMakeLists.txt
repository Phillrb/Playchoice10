cmake_minimum_required (VERSION 2.6)
project (PC10)

# The version number
set (PC10_VERSION_MAJOR 1)
set (PC10_VERSION_MINOR 0)

# c++11
if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    endif ()
else ()
    set (CMAKE_CXX_STANDARD 11)
endif ()

# Headers
set (HDR
    ${PROJECT_SOURCE_DIR}/util;
    ${PROJECT_SOURCE_DIR}/zip;
) 

# Source files
set (SRC 
    ${PROJECT_SOURCE_DIR}/PC10.cpp; 
    ${PROJECT_SOURCE_DIR}/util/checksum.cpp;
    ${PROJECT_SOURCE_DIR}/util/rp5h01_device.cpp; 
    ${PROJECT_SOURCE_DIR}/util/util.cpp; 
    ${PROJECT_SOURCE_DIR}/zip/miniz.cpp;
)

# Compile / link instructions
include_directories ("${HDR}")
add_executable(PC10 "${SRC}" )