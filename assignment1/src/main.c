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

    color_map_entry_t normal_colors[] = {
        {.height = 0, .color = (Color){27, 136, 204}},
        {.height = 64, .color = (Color){232, 217, 162}},
        {.height = 75, .color = (Color){221, 237, 114}},
        {.height = 85, .color = (Color){136, 217, 89}},
        {.height = 130, .color = (Color){56, 153, 37}},
        {.height = 180, .color = (Color){216, 242, 239}},
    };

    // Main game loop
    uint8_t seed = 0;
    while (!WindowShouldClose()) {
        if (true) {
            Heightmap_GeneratePerlin(&map, seed++);
            Heightmap_ApplyEdgeMixing(&map, falloff);
            Heightmap_ApplySmarterErosion(&map);
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            if (IsKeyDown(KEY_S))
                Heightmap_DrawGray(&map, 5);
            else
                Heightmap_Draw(&map, 5, normal_colors, 6);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}