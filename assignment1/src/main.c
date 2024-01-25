#include <raylib.h>

#include "generation.h"

int main() {
    const Vector2 screen_size = {1024, 768};
    InitWindow(screen_size.x, screen_size.y, "Assignment 1");

    SetTargetFPS(60);

    // Data
    heightmap_t map = {0};
    Heightmap_Create(&map, 100, 100);
    Heightmap_GeneratePerlin(&map);
    Heightmap_ApplyEdgeMixing(&map);

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            Heightmap_Draw(&map, 5);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}