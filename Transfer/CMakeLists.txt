file(
        GLOB_RECURSE TRANSFER_SOURCES
        CONFIGURE_DEPENDS *.cpp *.h
)
add_library(TransferLib ${TRANSFER_SOURCES})
target_include_directories(
        TransferLib
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(
        TransferLib
        PUBLIC
        MMOLib
        TypesLib
        CONAN_PKG::libpqxx
        CONAN_PKG::openssl
)
