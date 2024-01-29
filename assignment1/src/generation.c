#include "generation.h"

#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include "stb_perlin.h"
#include "utils.h"

void Heightmap_Create(heightmap_t* map, uint32_t width, uint32_t height) {
    // calloc() initializes to 0 :)
    map->data = calloc(width * height, sizeof(float));
    map->width = width;
    map->height = height;
}

float Heightmap_Get(heightmap_t* map, uint32_t x, uint32_t y) {
    x = Clamp(x, 0, map->width - 1);
    y = Clamp(y, 0, map->height - 1);
    return map->data[y * map->width + x];
}

void Heightmap_Set(heightmap_t* map, uint32_t x, uint32_t y, float value) {
    x = Clamp(x, 0, map->width - 1);
    y = Clamp(y, 0, map->height - 1);
    map->data[y * map->width + x] = Clamp(value, 0.f, 1.f);
}

void Heightmap_Depress(heightmap_t* map, uint32_t x, uint32_t y, float value, uint32_t macro_size) {
    for (int32_t xx = -macro_size; xx <= macro_size; xx++)
        for (int32_t yy = -macro_size; yy <= macro_size; yy++) {
            float scalar = (1.f / powf(2.f, (abs(xx) + abs(yy)))); // 1/2^(manhattan distance)
            float value = Heightmap_Get(map, x + xx, y + yy);
            Heightmap_Set(map, x + xx, y + yy, value * scalar);
        }
}

void Heightmap_GeneratePerlin(heightmap_t* map, uint32_t seed) {
    for (uint32_t x = 0; x < map->width; x++)
        for (uint32_t y = 0; y < map->height; y++) {
            Heightmap_Set(
                map, x, y,
                stb_perlin_turbulence_noise3(x / 50.f + seed, y / 50.f + seed,
                                             0.f, 1.34f, 0.6f, 12));
        }
}

void Heightmap_ApplyEdgeMixing(heightmap_t* map, float falloff) {
    for (uint32_t x = 0; x < map->width; x++)
        for (uint32_t y = 0; y < map->height; y++) {
            float original_value = Heightmap_Get(map, x, y);
            float norm_x = ((float)x) / map->width;
            float norm_y = ((float)y) / map->height;
            float scalar = 1.f - Vector2DistanceSqr((Vector2){norm_x, norm_y},
                                                    (Vector2){0.5f, 0.5f}) *
                                     falloff;
            Heightmap_Set(map, x, y, original_value * scalar);
        }
}

IntVector2 Heightmap_FindLowestNeighbor(heightmap_t* map, IntVector2 point) {
    IntVector2 lowest = {0};
    float lowest_value = FLT_MAX;

    for (int32_t xx = -1; xx <= 1; xx++)
        for (int32_t yy = -1; yy <= 1; yy++) {
            if (xx == 0 && yy == 0) continue;

            float neighbor_value =
                Heightmap_Get(map, point.x + xx, point.y + yy);
            if (neighbor_value < lowest_value) {
                lowest = (IntVector2){point.x + xx, point.y + yy};
                lowest_value = neighbor_value;
            }
        }

    return lowest;
}

void Heightmap_ApplyNaiveErosion(heightmap_t* map) {
    for (uint32_t rain_iteration = 0; rain_iteration < 1000; rain_iteration++) {
        IntVector2 point = {GetRandomValue(0, map->width - 1),
                            GetRandomValue(0, map->height - 1)};

        const float single_drop_erosion = 0.001f;
        for (uint32_t erosion_iteration = 0; erosion_iteration < 100;
             erosion_iteration++) {
            float value = Heightmap_Get(map, point.x, point.y);
            Heightmap_Set(map, point.x, point.y, value - single_drop_erosion);
            point = Heightmap_FindLowestNeighbor(map, point);
        }
    }
}

void Heightmap_ApplySmarterErosion(heightmap_t* map) {
    for (uint32_t rain_iteration = 0; rain_iteration < 10000; rain_iteration++) {
        IntVector2 point = {GetRandomValue(0, map->width - 1),
                            GetRandomValue(0, map->height - 1)};
        Vector2 velocity = Vector2Zero();

        for (uint32_t erosion_iteration = 0; erosion_iteration < 1000;
             erosion_iteration++) {
            // Calculate surface normal
            float value = Heightmap_Get(map, point.x, point.y);
            float xd_value = (Heightmap_Get(map, point.x + 1, point.y) - value) - (Heightmap_Get(map, point.x - 1, point.y) - value);
            float yd_value = (Heightmap_Get(map, point.x, point.y + 1) - value) - (Heightmap_Get(map, point.x, point.y - 1) - value);

            const float velocity_scale = 50.f;
            velocity.x += -xd_value * velocity_scale;
            velocity.y += -yd_value * velocity_scale;
            // velocity.x = Clamp(velocity.x, -1.f, 1.f);
            // velocity.y = Clamp(velocity.y, -1.f, 1.f);

            IntVector2 last_point = point;
            const float motion_thresh = 0.1f;
            if (velocity.x > motion_thresh) point.x += 1;
            if (velocity.x < -motion_thresh) point.x -= 1;
            if (velocity.y > motion_thresh) point.y += 1;
            if (velocity.y < -motion_thresh) point.y -= 1;

            const float raindrop_power = 0.05f;
            const uint32_t raindrop_size = 3;
            Heightmap_Depress(map, last_point.x, last_point.y, raindrop_power, raindrop_size);

            const float water_level = 64.f/255.f;
            if (Heightmap_Get(map, point.x, point.y) < water_level) break; // We have reached the ocean
        }
    }
}

void Heightmap_DrawGray(heightmap_t* map, uint32_t tile_size) {
    for (uint32_t x = 0; x < map->width; x++)
        for (uint32_t y = 0; y < map->height; y++) {
            uint32_t value = Clamp(Heightmap_Get(map, x, y) * 255, 0, 255);
            DrawRectangle(x * tile_size, y * tile_size, tile_size, tile_size,
                          (Color){value, value, value, 255});
        }
}

void Heightmap_Draw(heightmap_t* map, uint32_t tile_size) {
    for (uint32_t x = 0; x < map->width; x++)
        for (uint32_t y = 0; y < map->height; y++) {
            uint32_t value = Clamp(Heightmap_Get(map, x, y) * 255, 0, 255);
            Color color = PINK;
            if (value > 160) {
                color = (Color){225, 235, 245, 255};
            } else if (value > 130) {
                color = (Color){216, 242, 121, 255};
            } else if (value > 110) {
                color = (Color){87, 176, 76, 255};
            } else if (value > 80) {
                color = (Color){36, 102, 37, 255};
            } else if (value > 64) {
                color = (Color){232, 200, 137, 255};
            } else {
                color = (Color){77, 129, 189, 255};
            }
            DrawRectangle(x * tile_size, y * tile_size, tile_size, tile_size,
                          color);
        }
}
