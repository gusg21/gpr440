#include "generation.h"

#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#include "stb_perlin.h"

void Heightmap_Create(heightmap_t* map, uint32_t width, uint32_t height) {
    // calloc() initializes to 0 :)
    map->data = calloc(width * height, sizeof(float));
    map->width = width;
    map->height = height;
}

float Heightmap_Get(heightmap_t* map, uint32_t x, uint32_t y) {
    return map->data[y * map->width + x];
}

void Heightmap_Set(heightmap_t* map, uint32_t x, uint32_t y, float value) {
    map->data[y * map->width + x] = value;
}

void Heightmap_GeneratePerlin(heightmap_t* map) {
    for (int x = 0; x < map->width; x++)
        for (int y = 0; y < map->height; y++) {
            Heightmap_Set(
                map, x, y, stb_perlin_turbulence_noise3(x / 10.f, y / 10.f, 0.f, 1.2f, 0.5f, 5));
        }
}

void Heightmap_ApplyEdgeMixing(heightmap_t* map) {}

void Heightmap_Draw(heightmap_t* map, uint32_t tile_size) {
    for (int x = 0; x < map->width; x++)
        for (int y = 0; y < map->height; y++) {
            uint32_t value = Heightmap_Get(map, x, y) * 255;
            DrawRectangle(x * tile_size, y * tile_size, tile_size, tile_size, (struct Color){value, value, value, 255});
        }
}
