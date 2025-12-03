#include <stdlib.h>
#include "raylib.h"

// Constants
const int screenWidth = 1000;
const int screenHeight = 1000;


// Function prototypes
static void UpdateDrawFrame(void); // Update and draw one frame


// Local variables
int gravity = 10;
char* name = "Nasim Muhtasebzada";

int main() {

    InitWindow(screenWidth, screenHeight, "raylib");

    SetTargetFPS(60); 
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    CloseWindow();

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void) {
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("This is a raylib example", 10, 40, 20, DARKGRAY);
        DrawText(name, 10, 100, 20, DARKGRAY);
        DrawFPS(10, 10);
    EndDrawing();
}
