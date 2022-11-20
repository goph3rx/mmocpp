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

# Enable lints
# See https://www.kitware.com/static-checks-with-cmake-cdash-iwyu-clang-tidy-lwyu-cpplint-and-cppcheck/
if (LINT)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy --warnings-as-errors=* -checks=-*,bugprone-*,clang-analyzer-*,concurrency-*,misc-*,modernize-*,performance-*,portability-*,-modernize-use-trailing-return-type,-modernize-avoid-c-arrays,-bugprone-easily-swappable-parameters,-bugprone-lambda-function-name,-misc-no-recursion)
    set(CMAKE_CXX_CPPCHECK cppcheck --std=c++17 --enable=style --error-exitcode=1 --suppress=localMutex --suppress=preprocessorErrorDirective)
endif()

# Libraries
add_subdirectory("Types")
add_subdirectory("MMO")
add_subdirectory("Account")
add_subdirectory("Test")
