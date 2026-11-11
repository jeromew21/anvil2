cmake_minimum_required(VERSION 3.20)
project(anvil2-kit VERSION 0.1.0 LANGUAGES CXX)

option(ANVIL2_BUILD_LUA "Build Lua scripting support" ON)
option(ANVIL2_BUILD_BOX2D "Build Box2D support" ON)
option(ANVIL2_BUILD_EXAMPLES "Build examples" ON)
option(ANVIL2_BUILD_DOCS "Build documentation" OFF)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(EMSCRIPTEN)
    set(PLATFORM_WEB TRUE)
    set(PLATFORM "Web")
endif()

# Core dependencies
set(IMGUI_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui-1.93.0")
set(RAYLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib-6.2")
set(RLIMGUI_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/rlImGui-Raylib_6_0")
set(FREETYPE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/freetype-2.15.0")
set(HARFBUZZ_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/harfbuzz-14.6.0")
set(FMT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/fmt-12.1.0")
set(PHYSFS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/physfs")
set(FRAMEFLOW_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/frameflow")
set(SPDLOG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog-1.17.0")

set(BUILD_EXAMPLES OFF)
set(BUILD_GAMES OFF)
add_subdirectory("${RAYLIB_PATH}")
unset(BUILD_EXAMPLES)
unset(BUILD_GAMES)

set(FT_DISABLE_PNG ON)
set(FT_DISABLE_BROTLI ON)
set(FT_DISABLE_BZIP2 ON)
set(FT_DISABLE_ZLIB ON)
add_subdirectory("${FREETYPE_PATH}")

add_subdirectory("${HARFBUZZ_PATH}")

set(PHYSFS_BUILD_STATIC ON)
set(PHYSFS_BUILD_SHARED OFF)
set(PHYSFS_BUILD_TEST OFF)
add_subdirectory("${PHYSFS_PATH}")

add_subdirectory("${FMT_PATH}")

add_subdirectory("${FRAMEFLOW_PATH}")

add_subdirectory("${SPDLOG_PATH}")


add_subdirectory(rust/frameflow2)

if (ANVIL2_BUILD_LUA)
    set(LUA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/Lua-5.4.7")
    set(SOL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/sol2")
    add_subdirectory("${LUA_PATH}")
    add_subdirectory("${SOL_PATH}")
endif()

if (ANVIL2_BUILD_BOX2D)
    set(BOX2D_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/box2d-3.2.0")
    add_subdirectory("${BOX2D_PATH}")
endif()

add_library(anvil-raylib-backend STATIC
    "${IMGUI_PATH}/imgui.h"
    "${IMGUI_PATH}/imgui.cpp"
    "${IMGUI_PATH}/imgui_widgets.cpp"
    "${IMGUI_PATH}/imgui_tables.cpp"
    "${IMGUI_PATH}/imgui_draw.cpp"
    "${IMGUI_PATH}/imgui_demo.cpp"
    "${IMGUI_PATH}/misc/cpp/imgui_stdlib.h"
    "${IMGUI_PATH}/misc/cpp/imgui_stdlib.cpp"
    "${IMGUI_PATH}/misc/freetype/imgui_freetype.h"
    "${IMGUI_PATH}/misc/freetype/imgui_freetype.cpp"
    "${RLIMGUI_PATH}/rlImGui.cpp"
)

target_include_directories(anvil-raylib-backend PUBLIC
    "${IMGUI_PATH}"
    "${IMGUI_PATH}/misc/cpp"
    "${RLIMGUI_PATH}"
)

target_link_libraries(anvil-raylib-backend PUBLIC raylib freetype harfbuzz)

set(ANVIL_SOURCES
    src/anvil2/math/vec2.cpp
    src/anvil2/random/random.cpp
    src/anvil2/tween/tween.cpp
)

add_library(anvil2 STATIC ${ANVIL_SOURCES})
target_compile_features(anvil2 PUBLIC cxx_std_17)

target_include_directories(anvil2 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    "${CMAKE_CURRENT_SOURCE_DIR}/external/include"
)

target_link_libraries(anvil2 PUBLIC
    anvil-raylib-backend
    frameflow::frameflow
    fmt::fmt
    PhysFS::PhysFS
    spdlog::spdlog
)

target_link_libraries(anvil2 PUBLIC frameflow_rs)

if (ANVIL2_BUILD_LUA)
    target_link_libraries(anvil2 PUBLIC lua_static sol2::sol2)
endif()

if (ANVIL2_BUILD_BOX2D)
    target_link_libraries(anvil2 PUBLIC box2d::box2d)
endif()

if (ANVIL2_BUILD_EXAMPLES)
    add_executable(hello-world examples/main.cpp)
    target_link_libraries(hello-world PRIVATE anvil2)
    add_executable(pong examples/pong.cpp)
    target_link_libraries(pong PRIVATE anvil2)
endif()

if (ANVIL2_BUILD_DOCS)
    find_package(Doxygen REQUIRED)

    add_custom_target(anvil2-docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating documentation"
        VERBATIM
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(anvil2 PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
