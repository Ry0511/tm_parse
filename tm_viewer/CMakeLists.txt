cmake_minimum_required(VERSION 3.23)
project(
    tm_viewer
    VERSION 1.0
)

################################################################################
# | TM_VIEWER |
################################################################################

add_executable(tm_viewer)

file(GLOB_RECURSE TM_VIEWER_SOURCE_FILES "*.cpp")
file(GLOB_RECURSE TM_VIEWER_HEADER_FILES "*.h")
message(STATUS "tm_viewer sources")
message(STATUS "${TM_VIEWER_SOURCE_FILES}" "${TM_VIEWER_HEADER_FILES}")
target_sources(tm_viewer PRIVATE ${TM_VIEWER_HEADER_FILES} ${TM_VIEWER_SOURCE_FILES})

target_link_libraries(tm_viewer PRIVATE tm_parse)

target_compile_features(tm_viewer PRIVATE cxx_std_20)
target_compile_options(
    tm_viewer
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
)

################################################################################
# | IMGUI & GLFW |
################################################################################

include(FetchContent)
fetchcontent_declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

fetchcontent_declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.4-docking
)

fetchcontent_makeavailable(glfw imgui)

target_sources(tm_viewer PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(tm_viewer PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

find_package(OpenGL REQUIRED)
target_link_libraries(tm_viewer PRIVATE glfw OpenGL::GL)
