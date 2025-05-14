cmake_minimum_required(VERSION 3.10)
project(mygrep CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    main.cpp
    src/arg_parser.cpp
    src/file_reader.cpp
    src/matcher.cpp
    src/line_processor.cpp
    src/output.cpp
    src/grep_error.cpp
)

# Create executable
add_executable(mygrep ${SOURCES})

# Compiler flags
add_compile_options(-Wall)

# Link filesystem library if needed (for GCC < 9)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(mygrep PRIVATE stdc++fs)
endif()