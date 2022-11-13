file(
        GLOB_RECURSE MMO_SOURCES
        CONFIGURE_DEPENDS *.cpp *.h
)
add_library(MMOLib ${MMO_SOURCES})
target_include_directories(
        MMOLib
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(
        MMOLib
        PUBLIC
        CONAN_PKG::asio
        CONAN_PKG::icu
        CONAN_PKG::protobuf
)
