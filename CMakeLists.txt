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

# Core dependencies
set(IMGUI_PATH external/imgui-1.93.0)
set(RAYLIB_PATH external/raylib-6.2)
set(RLIMGUI_PATH external/rlImGui-Raylib_6_0)

#add_subdirectory(external/freetype)
#add_subdirectory(external/harfbuzz)
#add_subdirectory(external/fmt)
#add_subdirectory(external/concurrentqueue)
#add_subdirectory(external/frameflow)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
add_subdirectory(${RAYLIB_PATH})
unset(BUILD_EXAMPLES)
unset(BUILD_GAMES)

# Scripting languages
# set(LUA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/lua-5.4.9)
# set(SOL_PATH external/sol2)
# add_subdirectory(${SOL_PATH})

# add_library(lua STATIC IMPORTED)
# if (UNIX)
#     set_target_properties(lua PROPERTIES
#         IMPORTED_LOCATION "${LUA_PATH}/src/liblua.a"
#         INTERFACE_INCLUDE_DIRECTORIES "${LUA_PATH}/src"
#     )
# endif()

add_library(anvil-raylib-backend STATIC
        ${IMGUI_PATH}/imgui.h
        ${IMGUI_PATH}/imgui.cpp
        ${IMGUI_PATH}/imgui_widgets.cpp
        ${IMGUI_PATH}/imgui_tables.cpp
        ${IMGUI_PATH}/imgui_draw.cpp
        ${IMGUI_PATH}/imgui_demo.cpp
        ${IMGUI_PATH}/misc/cpp/imgui_stdlib.h
        ${IMGUI_PATH}/misc/cpp/imgui_stdlib.cpp
        #${IMGUI_PATH}/misc/freetype/imgui_freetype.h
        #${IMGUI_PATH}/misc/freetype/imgui_freetype.cpp
        ${RLIMGUI_PATH}/rlImGui.cpp
)

target_include_directories(anvil-raylib-backend
        PUBLIC
        ${IMGUI_PATH}
        ${IMGUI_PATH}/misc/cpp
        ${RLIMGUI_PATH}
)

#target_link_libraries(anvil-raylib-backend PRIVATE freetype)
target_link_libraries(anvil-raylib-backend PUBLIC raylib)

set(ANVIL_SOURCES
    src/anvil.cpp
    src/math/vec2.cpp
)

add_library(anvil2 ${ANVIL_SOURCES})

target_include_directories(anvil2 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
    ${SOL_PATH/include}
)

# These are the annoying things that we want to fully abstract over
# and not expose to the API.
# target_link_libraries(anvil PRIVATE harfbuzz)

# These are available for the user to use, or drop into if they want.
target_link_libraries(anvil2 PUBLIC
        anvil-raylib-backend
  #      lua
  #      sol2::sol2
        #frameflow::frameflow
        #concurrentqueue
        #fmt::fmt
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
