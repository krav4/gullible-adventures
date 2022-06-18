#pragma once
#include "olcPixelGameEngine.h"
#include "config.h"
#include "creature.h"
#include <string.h>
#include <unordered_map>

#define LEVEL_DESIGN_N_TILES_X ((int)99)
#define LEVEL_DESIGN_N_TILES_Y ((int)16)

#define LEVEL_DESIGN_EMPTY '.'
#define LEVEL_DESIGN_GRASS 'G'
#define LEVEL_DESIGN_DIRT '#'
#define LEVEL_DESIGN_BLACK ' '
#define LEVEL_DESIGN_SKY '.'
#define LEVEL_DESIGN_LUPI 'L'

class Tile
{
public:
	char symbol;
	olc::vi2d n_pos;
	olc::vi2d size = { PX_TILE_SIZE_X, PX_TILE_SIZE_Y };

	Tile() 
	{
		symbol = ' ';
		n_pos = { 0, 0 };
	};
	Tile(char tile_symbol, olc::vi2d tile_pos)
	{
		symbol = tile_symbol;
		n_pos = tile_pos;
	}
};

class LevelDesigns
{
public:
	std::vector<std::wstring> designs;
	std::unordered_map<char, StaticCreature*> static_creatures;
	LevelDesigns()
	{
		std::wstring design_level_1;
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"................#####..............................................................................";
		design_level_1 += L"...............#.....#........#....................................................................";
		design_level_1 += L"..............#.......#.......#....................................................................";
		design_level_1 += L".............#.........#......#....................................................................";
		design_level_1 += L"........L...#...........#.....#....................................................................";
		design_level_1 += L"############.............####.########..#######....................................................";
		design_level_1 += L"............................#.#........#...........................................................";
		design_level_1 += L"............................#.#.......#............................................................";
		design_level_1 += L"............................#.#......#.............................................................";
		design_level_1 += L"............................##########.............................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";

		designs.push_back(design_level_1);
	}
	Tile get_level_tile(olc::vi2d pos, int level_id)
	{
		if (pos.x < LEVEL_DESIGN_N_TILES_X && pos.y < LEVEL_DESIGN_N_TILES_Y && pos.x >=0 && pos.y >=0)
		{
			return Tile(designs[level_id][(pos.x + LEVEL_DESIGN_N_TILES_X * pos.y)], pos);
		}
		else
		{
			return Tile(' ', pos);
		}
	}

	void add_static_creature(char symbol, StaticCreature* static_creature)
	{
		static_creatures[symbol] = static_creature;
	}

};