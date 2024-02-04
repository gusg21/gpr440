#include <raylib.h>
#include <stdint.h>

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
void Heightmap_Destroy(heightmap_t* map);
float Heightmap_Get(heightmap_t* map, uint32_t x, uint32_t y);
void Heightmap_Set(heightmap_t* map, uint32_t x, uint32_t y, float value);
void Heightmap_GeneratePerlin(heightmap_t* map, uint32_t seed, float scale,
                              float lacunarity, float gain, uint32_t octaves);
void Heightmap_ApplyEdgeMixing(heightmap_t* map, float falloff);
// void Heightmap_ApplyNaiveErosion(heightmap_t* map);
void Heightmap_ApplySmarterErosion(heightmap_t* map, uint32_t num_raindrops,
                                   float raindrop_power, uint32_t raindrop_size,
                                   float water_level);
void Heightmap_DrawGray(heightmap_t* map, uint32_t sx, uint32_t sy,
                        uint32_t tile_size);
void Heightmap_Draw(heightmap_t* map, uint32_t sx, uint32_t sy,
                    uint32_t tile_size, color_map_entry_t* color_map,
                    size_t num_colors);