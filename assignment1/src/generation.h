#include <stdint.h>
#include <raylib.h>


typedef struct heightmap {
    float* data;
    uint32_t width;
    uint32_t height;
} heightmap_t;

typedef struct color_map_entry {
    uint8_t height;
    Color color;
} color_map_entry_t;

void Heightmap_Create(heightmap_t* map, uint32_t width, uint32_t height);
float Heightmap_Get(heightmap_t* map, uint32_t x, uint32_t y);
void Heightmap_Set(heightmap_t* map, uint32_t x, uint32_t y, float value);
void Heightmap_GeneratePerlin(heightmap_t* map, uint32_t seed);
void Heightmap_ApplyEdgeMixing(heightmap_t* map, float falloff);
void Heightmap_ApplyNaiveErosion(heightmap_t* map);
void Heightmap_ApplySmarterErosion(heightmap_t* map);
void Heightmap_DrawGray(heightmap_t* map, uint32_t tile_size);
void Heightmap_Draw(heightmap_t* map, uint32_t tile_size,
                    color_map_entry_t* color_map, size_t num_colors);