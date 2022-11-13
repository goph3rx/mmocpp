cmake_minimum_required(VERSION 3.20)
project(mmo)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
find_package(Protobuf REQUIRED)

link_libraries(CONAN_PKG::spdlog)
add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
include(GoogleTest)
enable_testing()

# Enable warnings
if (MSVC)
    add_compile_options(
            /W4
            /WX
            /wd4018
    )
else()
    add_compile_options(
            -Wall
            -Wextra
            -pedantic
            -Werror
            -Wno-sign-compare
            -Wno-gnu-zero-variadic-macro-arguments
            -Wno-unused-private-field
    )
endif()

# Libraries
add_subdirectory("MMO")
add_subdirectory("Test")
