cmake_minimum_required ( VERSION 2.6 )
project ( PC10 )

# The version number
set ( PC10_VERSION_MAJOR 1 )
set ( PC10_VERSION_MINOR 0 )

# c++11
set ( CMAKE_CXX_STANDARD 11 )

# Headers
set ( HDR
    ${PROJECT_SOURCE_DIR}/util;
    ${PROJECT_SOURCE_DIR}/zip;
) 

# Source files
set ( SRC 
    ${PROJECT_SOURCE_DIR}/PC10.cpp; 
    ${PROJECT_SOURCE_DIR}/util/checksum.cpp;
    ${PROJECT_SOURCE_DIR}/util/rp5h01_device.cpp; 
    ${PROJECT_SOURCE_DIR}/util/util.cpp; 
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