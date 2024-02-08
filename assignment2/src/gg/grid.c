#include "grid.h"

#include <stdlib.h>

static uint32_t clamp(uint32_t x, uint32_t min, uint32_t max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

void Grid_Create(gg_grid_t* grid, uint32_t width, uint32_t height) {
    grid->data = calloc(width * height, sizeof(uint32_t));
    grid->width = width;
    grid->height = height;
}

void Grid_Destroy(gg_grid_t* grid) { free(grid->data); }

void Grid_Set(gg_grid_t* grid, uint32_t x, uint32_t y, uint32_t data) {
    x = clamp(x, 0, grid->width - 1);
    y = clamp(y, 0, grid->height - 1);
    grid->data[x + y * grid->width] = data;
}

uint32_t Grid_Get(gg_grid_t* grid, uint32_t x, uint32_t y) {
    return grid->data[x + y * grid->width];
}
