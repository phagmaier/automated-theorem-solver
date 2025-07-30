cmake_minimum_required(VERSION 3.15)
project(Solver CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(runme
  src/main.cpp
  src/helpers.cpp
  src/lexer.cpp
  src/token.cpp
  src/clause.cpp
  src/solver.cpp
  src/display_tree.cpp
  src/tree.cpp
  src/dynamicbitset.cpp
)

target_include_directories(runme PRIVATE src)

set_target_properties(runme PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_compile_options(runme PRIVATE
  -Wall
  -Wextra
)
