cmake_minimum_required(VERSION 3.30)
project(tm_parse VERSION 1.0.0)

# TODO: Extract this out into toolchain files
################################################################################
# | MSVC CRAP |
################################################################################

if (NOT DEFINED CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER cl.exe)
    set(CMAKE_CXX_COMPILER cl.exe)
endif ()

add_compile_options("/utf-8")

################################################################################
# | TM_PARSE_MAIN |
################################################################################

add_executable(tm_parse_main src/main.cpp)
target_link_libraries(tm_parse_main PRIVATE tm_parse)
target_compile_features(tm_parse_main PRIVATE cxx_std_20)

################################################################################
# | TARGET TM_PARSE |
################################################################################

file(GLOB_RECURSE TM_PARSE_SOURCE_FILES "src/tm_parse/*.cpp")
file(GLOB_RECURSE TM_PARSE_HEADER_FILES "src/tm_parse/*.h")

message(STATUS "tm_parse source files '${TM_PARSE_SOURCE_FILES}'")
message(STATUS "tm_parse header files '${TM_PARSE_HEADER_FILES}'")

add_library(tm_parse STATIC ${TM_PARSE_SOURCE_FILES})
target_include_directories(tm_parse PUBLIC src/)
target_precompile_headers(tm_parse PRIVATE src/tm_parse/pch.h)
target_compile_features(tm_parse PRIVATE cxx_std_20)