file(
        GLOB_RECURSE TEST_SOURCES
        CONFIGURE_DEPENDS *.cpp *.h
)
add_executable(Tests ${TEST_SOURCES})
target_link_libraries(
        Tests
        PRIVATE
        AccountLib
        MMOLib
        CONAN_PKG::gtest
        CONAN_LIB::gtest_gmock
)

gtest_discover_tests(Tests)
add_test(
        NAME Tests
        COMMAND Tests
)

# Disable linting of tests
set_property(
        TARGET Tests
        PROPERTY CXX_CLANG_TIDY
)