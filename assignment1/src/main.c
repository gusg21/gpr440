#include <raylib.h>

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "generation.h"
#include "raylib-nuklear.h"

typedef struct gen_settings {
    uint32_t seed;

    float falloff;
    float scale;
    float lacunarity;
    float gain;
    uint32_t octaves;

    uint32_t num_raindrops;
    float raindrop_power;
    uint32_t raindrop_size;
    float water_level;
} gen_settings_t;

void GenerateHeightmap(heightmap_t* heightmap, gen_settings_t* settings) {
    // Apply the settings in the structure to the map's generation
    Heightmap_GeneratePerlin(heightmap, settings->seed, settings->scale,
                             settings->lacunarity, settings->gain,
                             settings->octaves);
    Heightmap_ApplyEdgeMixing(heightmap, settings->falloff);
    Heightmap_ApplySmarterErosion(
        heightmap, settings->num_raindrops, settings->raindrop_power,
        settings->raindrop_size, settings->water_level);
}

void ApplyStyle(struct nk_context* ctx) {
    // Green text on buttons
    ctx->style.button.text_normal = (struct nk_color){62, 230, 107, 255};
    ctx->style.button.text_active = (struct nk_color){42, 210, 87, 255};
    ctx->style.button.text_hover = (struct nk_color){92, 255, 127, 255};
}

void ConstructSettingsEditor(struct nk_context* ctx, heightmap_t* heightmap,
                             gen_settings_t* settings) {
    if (nk_begin(
            ctx, "Settings Editor", nk_rect(20, 20, 320, 520),
            NK_WINDOW_MINIMIZABLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE)) {
        // Generation Settings
        nk_layout_row_dynamic(ctx, 250, 1);
        if (nk_group_begin_titled(ctx, "Generation", "Generation",
                                  NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
            nk_layout_row_dynamic(ctx, 25, 2);
            nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT | NK_TEXT_ALIGN_MIDDLE,
                      "Falloff: %.2f", settings->falloff);
            nk_slider_float(ctx, 0.f, &settings->falloff, 10.f, 0.1f);

            nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT | NK_TEXT_ALIGN_MIDDLE,
                      "Scale: %.2f", settings->scale);
            nk_slider_float(ctx, 5.f, &settings->scale, 200.f, 5.f);

            nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT | NK_TEXT_ALIGN_MIDDLE,
                      "Lacunarity: %.2f", settings->lacunarity);
            nk_slider_float(ctx, 1.f, &settings->lacunarity, 3.f, 0.1f);

            nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT | NK_TEXT_ALIGN_MIDDLE,
                      "Gain: %.2f", settings->gain);
            nk_slider_float(ctx, 0.f, &settings->gain, 3.f, 0.1f);

            nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT | NK_TEXT_ALIGN_MIDDLE,
                      "Octaves: %d", settings->octaves);
            nk_slider_int(ctx, 1, &settings->octaves, 50, 1);

            nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT | NK_TEXT_ALIGN_MIDDLE,
                      "Seed: %d", settings->seed);
            if (nk_button_symbol_text(ctx, NK_SYMBOL_TRIANGLE_RIGHT, "Randomize", 9, NK_TEXT_ALIGN_RIGHT)) {
                settings->seed = rand();
            }
        }
        nk_group_end(ctx);

        // Erosion Settings
        nk_layout_row_dynamic(ctx, 150, 1);
        if (nk_group_begin_titled(ctx, "Erosion", "Erosion",
                                  NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
            nk_layout_row_dynamic(ctx, 25, 2);
            nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT | NK_TEXT_ALIGN_MIDDLE,
                      "# of Raindrops: %d", settings->num_raindrops);
            nk_slider_int(ctx, 0, &settings->num_raindrops, 10000, 1000);

            nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT | NK_TEXT_ALIGN_MIDDLE,
                      "Raindrop Power: %.3f", settings->raindrop_power);
            nk_slider_float(ctx, 0.f, &settings->raindrop_power, 0.1f, 0.002f);

            nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT | NK_TEXT_ALIGN_MIDDLE,
                      "Raindrop Size: %d", settings->raindrop_size);
            nk_slider_int(ctx, 0, &settings->raindrop_size, 10, 1);
        }
        nk_group_end(ctx);

        // Button
        nk_layout_row_dynamic(ctx, 50, 1);
        if (nk_button_label(ctx, "Generate!")) {
            GenerateHeightmap(heightmap, settings);
        }
    }
    nk_end(ctx);
}

int main() {
    // Setup Window
    uint32_t screen_width = 1024;
    uint32_t screen_height = 768;

    InitWindow(screen_width, screen_height, "Assignment 1");
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    // Setup GUI
    struct nk_context* ctx = InitNuklear(10);
    ApplyStyle(ctx);

    // Properties
    gen_settings_t settings = {.seed = GetTime(),
                               .falloff = 3.0f,
                               .scale = 100.f,
                               .lacunarity = 1.34f,
                               .gain = 0.6f,
                               .octaves = 12,
                               .num_raindrops = 2000,
                               .raindrop_power = 0.007f,
                               .raindrop_size = 2,
                               .water_level = 64.f / 255.f};

    // Data
    heightmap_t map = {0};
    Heightmap_Create(&map, 170, 130);
    GenerateHeightmap(&map, &settings);

    // The color table to render the map with
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
        UpdateNuklear(ctx);

        // Show the user settings for manipulating the map gen
        ConstructSettingsEditor(ctx, &map, &settings);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            // Determine where to draw the map
            const int tile_size = 5;
            float map_x =
                GetRenderWidth() / 2.f - (map.width * tile_size) / 2.f;
            float map_y =
                GetRenderHeight() / 2.f - (map.height * tile_size) / 2.f;
            if (IsKeyDown(KEY_S)) {
                Heightmap_DrawGray(&map, map_x, map_y, tile_size);
            } else {
                Heightmap_Draw(&map, map_x, map_y, tile_size, normal_colors, 6);
            }

            // Info
            DrawText("Hold [S] to view the raw heightmap", 10, 10, 10, BLACK);
            DrawText("World generation demo with turbulent noise and erosion by Angus Goucher, 2024", 10, 23, 10, GRAY);

            // Draw the Gui
            DrawNuklear(ctx);
        }
        EndDrawing();
    }

    Heightmap_Destroy(&map);

    UnloadNuklear(ctx);
    CloseWindow();

    return 0;
}