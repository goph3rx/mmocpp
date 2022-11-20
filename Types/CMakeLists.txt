file(
        GLOB_RECURSE TYPES_SOURCES
        CONFIGURE_DEPENDS *.proto
)
add_library(TypesLib ${TYPES_SOURCES})
protobuf_generate_cpp(
        PROTO_SRCS PROTO_HDRS
        TARGET TypesLib
)

target_include_directories(
        TypesLib
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
target_link_libraries(TypesLib PUBLIC CONAN_PKG::protobuf)

# Disable linting of generated code
set_property(
        TARGET TypesLib
        PROPERTY CXX_CLANG_TIDY
)
set_property(
        TARGET TypesLib
        PROPERTY CMAKE_CXX_INCLUDE_WHAT_YOU_USE
)
