#pragma once
#include "config.h"
#include "olcPixelGameEngine.h"

class Tile
{
public:
	char symbol;
	olc::vi2d n_pos;
	olc::vi2d size = { PX_TILE_SIZE_X, PX_TILE_SIZE_Y };

	Tile();
	Tile(char tile_symbol, olc::vi2d tile_pos);
};