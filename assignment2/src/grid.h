#if !defined(GRID_H)
#define GRID_H

#include <stdint.h>

typedef struct grid {
    uint32_t* data;
    uint32_t width, height;
} grid_t;

void Grid_Create(grid_t* grid, uint32_t width, uint32_t height);
void Grid_Destroy(grid_t* grid);
void Grid_Set(grid_t* grid, uint32_t x, uint32_t y, uint32_t data);
uint32_t Grid_Get(grid_t* grid, uint32_t x, uint32_t y);

#endif  // GRID_H
