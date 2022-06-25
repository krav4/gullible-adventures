#pragma once

#define PX_WINDOW_SIZE_X 1500
#define PX_WINDOW_SIZE_Y 900

#define PX_TILE_SIZE_X 100
#define PX_TILE_SIZE_Y 100

#define LEVEL_DESIGN_N_TILES_X ((int)99)
#define LEVEL_DESIGN_N_TILES_Y ((int)16)

#define LEVEL_DESIGN_EMPTY '.'
#define LEVEL_DESIGN_GRASS 'G'
#define LEVEL_DESIGN_DIRT '#'
#define LEVEL_DESIGN_BLACK ' '
#define LEVEL_DESIGN_SKY '.'
#define LEVEL_DESIGN_CLOUD 'C'
#define LEVEL_DESIGN_LUPI 'L'
#define LEVEL_DESIGN_TRASHCAN 'T'
#define LEVEL_DESIGN_EXIT 'E'
#define LEVEL_DESIGN_DEATH 'D'

static constexpr float gravity = 70.0f;
