file(
        GLOB_RECURSE ACCOUNT_SOURCES
        CONFIGURE_DEPENDS *.cpp *.h
)
add_library(AccountLib ${ACCOUNT_SOURCES})
target_include_directories(
        AccountLib
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(
        AccountLib
        PUBLIC
        MMOLib
        TypesLib
        CONAN_PKG::libpqxx
        CONAN_PKG::openssl
)
