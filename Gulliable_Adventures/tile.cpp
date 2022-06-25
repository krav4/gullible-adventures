#include "tile.h"
#include "olcPixelGameEngine.h"
Tile::Tile()
{
	symbol = ' ';
	n_pos = { 0, 0 };
};

Tile::Tile(char tile_symbol, olc::vi2d tile_pos)
{
	symbol = tile_symbol;
	n_pos = tile_pos;
}
