cmake_minimum_required(VERSION 3.15)
project(Solver CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Correctly specify the path to your source files in the 'src' directory
add_executable(runme
    src/main.cpp
    src/helpers.cpp
    src/lexer.cpp
    src/token.cpp
    src/solver.cpp
)

# Apply include directories to the `runme` target
# This tells the compiler where to look for your header files (e.g., helpers.h)
target_include_directories(runme PRIVATE src)

set_target_properties(runme PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_compile_options(runme PRIVATE
    -Wall
    -Wextra
)
