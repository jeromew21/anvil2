/**
 * Main example using Raylib, ImGui, and anvil2 utilities
 * Demonstrates:
 * - Raylib window creation
 * - ImGui integration
 * - Lua integration
 * - anvil2 utilities (signal, defer, random)
 */
#include "anvil2/signal/signal.hpp"
#include "anvil2/defer/defer.hpp"
#include "anvil2/random/random.hpp"

#include <ankerl/unordered_dense.h>
#include <moodycamel/concurrentqueue.h>


#include <raylib.h>
#include <rlImGui.h>	// include the API header
#include <imgui.h>

#include <fmt/core.h>

#include <sol/sol.hpp>

int main(int argc, char* argv[]) {
    fmt::println("Hello, World!");
    moodycamel::ConcurrentQueue<int> q;
    q.enqueue(25);

    rnd::seed(0);

    Signal<> signal;

    Defer defer([&]{
        fmt::println("Deferred execution");

        for (int i = 0; i < 10; ++i) {
            fmt::println("Random number {}: {}", i, rnd::random_integer(0, 100));
        }

        signal.emit();
    });

    sol::state lua;
    int x = 0;
    lua.set_function("beep", [&x]{ ++x; fmt::println("fooo {}", rnd::random()); });
    lua.script("beep()");
    assert(x == 1);

    int item;
    bool found = q.try_dequeue(item);
    assert(found && item == 25);

    int id = 0;

    const int screenWidth = 800;
    const int screenHeight = 450;

    signal.connect([]{
        fmt::println("Signal triggered");
    });
    signal.connect([]{
        fmt::println("Signal triggered (2)");
    });

    signal.emit();

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    rlImGuiSetup(true); 	// sets up ImGui with ether a dark or light default theme
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            rlImGuiBegin();			// starts the ImGui content mode. Make all ImGui calls after this
            {
                ImGui::Begin("hello");
                ImGui::End();
                ClearBackground(RAYWHITE);
                DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            }
            rlImGuiEnd();			// ends the ImGui content mode. Make all ImGui calls before this
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    rlImGuiShutdown();		// cleans up ImGui
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
