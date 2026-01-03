cmake_minimum_required(VERSION 3.23)
project(tm_parse VERSION 1.0.0)

################################################################################
# | TM_PARSE_MAIN |
################################################################################

add_executable(tm_parse_main src/main.cpp)
target_link_libraries(tm_parse_main PRIVATE tm_parse)
target_compile_features(tm_parse_main PRIVATE cxx_std_20)

# Test Main
add_executable(tm_parse_tests)

file(GLOB_RECURSE TM_PARSE_TESTS_SOURCE_FILES "src/test/src/*.cpp")
file(GLOB_RECURSE TM_PARSE_TESTS_HEADER_FILES "src/test/src/*.h")
target_sources(tm_parse_tests PRIVATE ${TM_PARSE_TESTS_SOURCE_FILES} ${TM_PARSE_TESTS_SOURCE_FILES})

target_include_directories(tm_parse_tests PRIVATE src/test/src)
target_link_libraries(tm_parse_tests PRIVATE tm_parse)
target_precompile_headers(tm_parse_tests PRIVATE src/tm_parse/pch.h)
target_compile_features(tm_parse_tests PRIVATE cxx_std_20)

################################################################################
# | TARGET TM_PARSE |
################################################################################

file(GLOB_RECURSE TM_PARSE_SOURCE_FILES "src/tm_parse/*.cpp")
file(GLOB_RECURSE TM_PARSE_HEADER_FILES "src/tm_parse/*.h")

message(STATUS "tm_parse source files '${TM_PARSE_SOURCE_FILES}'")
message(STATUS "tm_parse header files '${TM_PARSE_HEADER_FILES}'")

add_library(tm_parse STATIC)

if (${TM_PARSE_ARCH} STREQUAL "x86")
    target_compile_definitions(tm_parse PUBLIC -DTM_PARSE_ARCH_X86=1)
elseif (${TM_PARSE_ARCH} STRLESS_EQUAL "x64")
    target_compile_definitions(tm_parse PUBLIC -DTM_PARSE_ARCH_X64=1)
endif ()

target_include_directories(tm_parse PUBLIC src/)
target_compile_features(tm_parse PRIVATE cxx_std_20)
target_compile_options(
        tm_parse
        PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
)
target_precompile_headers(tm_parse PRIVATE src/tm_parse/pch.h)

set(CONFIGURE_FILES_DIR "${CMAKE_CURRENT_BINARY_DIR}/configure")
set(PRE_CONFIGURE_FILE "src/tm_parse/git.inl.in")
set(POST_CONFIGURE_FILE "${CONFIGURE_FILES_DIR}/tm_parse/git.inl")
include(cmake_crap/git_watcher.cmake)

target_include_directories(tm_parse PRIVATE ${CONFIGURE_FILES_DIR})

if (WIN32)
    add_subdirectory(tm_viewer EXCLUDE_FROM_ALL)
endif ()

list(APPEND TM_PARSE_SOURCE_FILES "${CONFIGURE_FILES_DIR}/tm_parse/git.inl")
target_sources(tm_parse PRIVATE ${TM_PARSE_SOURCE_FILES} ${TM_PARSE_HEADER_FILES})