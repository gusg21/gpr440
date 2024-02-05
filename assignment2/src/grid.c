#include "grid.h"

#include <raymath.h>
#include <stdlib.h>

void Grid_Create(grid_t* grid, uint32_t width, uint32_t height) {
    grid->data = calloc(width * height, sizeof(uint32_t));
    grid->width = width;
    grid->height = height;
}

void Grid_Destroy(grid_t* grid) { free(grid->data); }

void Grid_Set(grid_t* grid, uint32_t x, uint32_t y, uint32_t data) {
    x = Clamp(x, 0, grid->width - 1);
    y = Clamp(y, 0, grid->height - 1);
    grid->data[x + y * grid->width] = data;
}

uint32_t Grid_Get(grid_t* grid, uint32_t x, uint32_t y) {
    return grid->data[x + y * grid->width];
}
