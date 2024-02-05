#include <raylib.h>
#include <stdint.h>

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "grid.h"
#include "raylib-nuklear.h"
#include "styling.h"

void ConstructSettingsEditor(struct nk_context* ctx) {
    if (nk_begin(
            ctx, "Settings Editor", nk_rect(300, 200, 320, 520),
            NK_WINDOW_MINIMIZABLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE)) {
        nk_label(ctx, "TBD", NK_TEXT_ALIGN_CENTERED);
    }
    nk_end(ctx);
}

void DrawGrid(grid_t* grid, uint32_t tile_size) {
    for (uint32_t x = 0; x < grid->width; x++) {
        for (uint32_t y = 0; y < grid->height; y++) {
            DrawRectangleLines(x * tile_size, y * tile_size, tile_size, tile_size, GRAY);
        }
    }
}

int main() {
    TraceLog(LOG_INFO, TextFormat("CWD: %s", GetWorkingDirectory()));

    // Setup Window
    uint32_t screen_width = 1024;
    uint32_t screen_height = 768;

    InitWindow(screen_width, screen_height, "Assignment 1");
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    MaximizeWindow();
    SetTargetFPS(60);

    // Setup GUI
    Font font = LoadFont("assets/roboto.ttf");
    const float font_size = 14.f;
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    struct nk_context* ctx = InitNuklearEx(font, font_size);
    ApplyStyle(ctx);

    // Data
    grid_t grid = {0};
    Grid_Create(&grid, 30, 30);

    while (!WindowShouldClose()) {
        UpdateNuklear(ctx);

        // Show the user settings for manipulating the map gen
        ConstructSettingsEditor(ctx);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            DrawGrid(&grid, 24);

            // Info
            DrawTextEx(font,
                       TextFormat("render %dx%d, screen %dx%d, dpi %.2f, %.2f",
                                  GetRenderWidth(), GetRenderHeight(),
                                  GetScreenWidth(), GetScreenHeight(),
                                  GetWindowScaleDPI().x, GetWindowScaleDPI().y),
                       (Vector2){10, 10}, font_size, 0, GRAY);
            DrawTextEx(font, TextFormat("FPS: %d", GetFPS()),
                       (Vector2){10, font_size * 2.f}, font_size, 0,
                       GetFPS() >= 50 ? GREEN : RED);

            // Draw the Gui
            DrawNuklear(ctx);
        }
        EndDrawing();
    }

    UnloadNuklear(ctx);
    CloseWindow();

    return 0;
}