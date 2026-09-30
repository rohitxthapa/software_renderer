cmake_minimum_required(VERSION 3.16)

project(software_renderer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

file(GLOB_RECURSE IMGUI_FILES
"dependency/imgui/*.cpp"
)

message(STATUS "Found Source Files: ${SRC_FILES}")
message(STATUS "Found ImGui Files: ${IMGUI_FILES}")

add_executable(renderer ${SRC_FILES} ${IMGUI_FILES})

target_include_directories(renderer PRIVATE
include
dependency/imgui
)

target_compile_options(renderer PRIVATE
    -Wall
    -Wextra
    -fsanitize=address,undefined
)

target_link_options(renderer PRIVATE
    -fsanitize=address,undefined
)

target_link_libraries(renderer PRIVATE
SDL3::SDL3
)
