cmake_minimum_required(VERSION 3.20)
project(mylib_zig NONE)  # no C/C++ compiler needed here

# does this need to be different for different platforms? maybe not, zig should handle that
set(MYLIB_LIB ${CMAKE_CURRENT_SOURCE_DIR}/zig-out/lib/libframeflow.a)

add_custom_command(
    OUTPUT ${MYLIB_LIB}
    COMMAND zig build -Doptimize=ReleaseFast
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/root.zig ${CMAKE_CURRENT_SOURCE_DIR}/build.zig
    COMMENT "Building Zig library"
)
add_custom_target(frameflow-zig-build DEPENDS ${MYLIB_LIB})

add_library(frameflow-zig STATIC IMPORTED GLOBAL)
set_target_properties(frameflow-zig PROPERTIES IMPORTED_LOCATION ${MYLIB_LIB})
target_include_directories(frameflow-zig INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_dependencies(frameflow-zig frameflow-zig-build)
