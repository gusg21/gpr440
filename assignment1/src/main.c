#include <raylib.h>

#include "generation.h"

int main() {
    struct IntVector2 { uint32_t x; uint32_t y; } screen_size = {1024, 768};
    InitWindow(screen_size.x, screen_size.y, "Assignment 1");

    SetTargetFPS(60);

    float falloff = 3.0f;

    // Data
    heightmap_t map = {0};
    Heightmap_Create(&map, 170, 130);
    Heightmap_GeneratePerlin(&map, 1719);
    Heightmap_ApplyEdgeMixing(&map, falloff);
    Heightmap_ApplySmarterErosion(&map);

    // Main game loop
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_A)) {
            Heightmap_GeneratePerlin(&map, GetTime());
            Heightmap_ApplyEdgeMixing(&map, falloff);
            Heightmap_ApplySmarterErosion(&map);
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            if (IsKeyDown(KEY_S))
                Heightmap_DrawGray(&map, 5);
            else
                Heightmap_Draw(&map, 5);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}