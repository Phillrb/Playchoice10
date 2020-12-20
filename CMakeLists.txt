cmake_minimum_required ( VERSION 3.1 )
project ( PC10 LANGUAGES CXX C)

# The version number
set ( PC10_VERSION_MAJOR 1 )
set ( PC10_VERSION_MINOR 0 )

# c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Headers
set ( HDR
    ${PROJECT_SOURCE_DIR}/util;
    ${PROJECT_SOURCE_DIR}/util/xgetopt;
    ${PROJECT_SOURCE_DIR}/zip;
) 

# Source files
set ( SRC 
    ${PROJECT_SOURCE_DIR}/PC10/PC10.cpp; 
    ${PROJECT_SOURCE_DIR}/util/checksum.cpp;
    ${PROJECT_SOURCE_DIR}/util/rp5h01_device.cpp; 
    ${PROJECT_SOURCE_DIR}/util/util.cpp; 
    ${PROJECT_SOURCE_DIR}/util/xgetopt/xgetopt.c;
    ${PROJECT_SOURCE_DIR}/zip/miniz.cpp;
)

# Compile / link instructions
include_directories ( "${HDR}" )
add_executable( PC10 "${SRC}" )

if( WIN32 )
    # Not everyone is a dev!
    # Assume users don't have these dlls on their machines
    target_link_libraries( PC10 -static-libgcc -static-libstdc++ )
endif()