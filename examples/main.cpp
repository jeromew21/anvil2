#include "entity/base_entity.hpp"


#include <raylib.h>
#include <rlImGui.h>	// include the API header
#include <imgui.h>

struct ItemContainer {
    bool isFreezer;
    double weightCapacity;
    std::array<int, 1000> items;
    std::array<bool, 1000> mask;
};

struct ItemList {
    std::vector<int> items;
    std::vector<double> grams;
};

int main(int argc, char* argv[]) {
    int id = 0;

    const int screenWidth = 800;
    const int screenHeight = 450;

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

	return 0;
}
