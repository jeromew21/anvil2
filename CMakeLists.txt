cmake_minimum_required(VERSION 3.20)
project(anvil2-kit VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(EMSCRIPTEN)
    set(PLATFORM_WEB TRUE)
    set(PLATFORM "Web")
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

option(ANVIL2_BUILD_LUA "Build Lua scripting support" OFF)

# Core dependencies
set(IMGUI_PATH external/imgui-1.93.0)
set(RAYLIB_PATH external/raylib-6.2)
set(RLIMGUI_PATH external/rlImGui-Raylib_6_0)
set(FREETYPE_PATH external/freetype-2.15.0)
set(HARFBUZZ_PATH external/harfbuzz-14.6.0)
set(FMT_PATH external/fmt-12.2.0)
set(PHYSFS_PATH external/physfs)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
add_subdirectory(${RAYLIB_PATH})
unset(BUILD_EXAMPLES)
unset(BUILD_GAMES)

set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
add_subdirectory(${FREETYPE_PATH})

add_subdirectory(${HARFBUZZ_PATH})

set(PHYSFS_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(PHYSFS_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(PHYSFS_BUILD_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(${PHYSFS_PATH})

add_subdirectory(${FMT_PATH})

add_subdirectory(external/frameflow)

if (ANVIL2_BUILD_LUA)
    set(LUA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/lua-5.4.9)
    set(SOL_PATH external/sol2)
    add_subdirectory(${SOL_PATH})

    # Make sure you build lua inside external/lua
    # not sure what to do tbh, will need to look at this
    add_library(lua STATIC IMPORTED)
    if (UNIX)
        set_target_properties(lua PROPERTIES
            IMPORTED_LOCATION "${LUA_PATH}/src/liblua.a"
            INTERFACE_INCLUDE_DIRECTORIES "${LUA_PATH}/src"
        )
    endif()
endif()

add_library(anvil-raylib-backend STATIC
        ${IMGUI_PATH}/imgui.h
        ${IMGUI_PATH}/imgui.cpp
        ${IMGUI_PATH}/imgui_widgets.cpp
        ${IMGUI_PATH}/imgui_tables.cpp
        ${IMGUI_PATH}/imgui_draw.cpp
        ${IMGUI_PATH}/imgui_demo.cpp
        ${IMGUI_PATH}/misc/cpp/imgui_stdlib.h
        ${IMGUI_PATH}/misc/cpp/imgui_stdlib.cpp
        ${IMGUI_PATH}/misc/freetype/imgui_freetype.h
        ${IMGUI_PATH}/misc/freetype/imgui_freetype.cpp
        ${RLIMGUI_PATH}/rlImGui.cpp
)

target_include_directories(anvil-raylib-backend
        PUBLIC
        ${IMGUI_PATH}
        ${IMGUI_PATH}/misc/cpp
        ${RLIMGUI_PATH}
)

target_link_libraries(anvil-raylib-backend PUBLIC raylib freetype harfbuzz)

set(ANVIL_SOURCES
    src/anvil.cpp
    src/math/vec2.cpp
    src/filesystem/filesystem.cpp
)

add_library(anvil2 ${ANVIL_SOURCES})

target_include_directories(anvil2 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
    ${SOL_PATH/include}
    external/include
)

# These are the annoying things that we want to fully abstract over
# and not expose to the API.
# target_link_libraries(anvil2 PRIVATE harfbuzz)

# These are available for the user to use, or drop into if they want.
target_link_libraries(anvil2 PUBLIC
        anvil-raylib-backend
        frameflow
        fmt::fmt
        PhysFS::PhysFS
  #      lua
  #      sol2::sol2
)

add_executable(hello-world examples/main.cpp)
target_link_libraries(hello-world PRIVATE anvil2)

if (UNIX)
    target_compile_options(anvil2 PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
endif()
