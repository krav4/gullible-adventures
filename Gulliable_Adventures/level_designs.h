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
#define LEVEL_DESIGN_DEATH 'D'

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

struct Level
{
	std::wstring design;
	olc::vf2d player_init_position;
};

class LevelDesigns
{
private:
	std::vector<Level> levels;
public:
	std::unordered_map<char, StaticCreature*> static_creatures;
	LevelDesigns()
	{
		std::wstring design_level_1;
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"..L............L...................................................................................";
		design_level_1 += L"#########################################....#######..##########################..#################";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";

		levels.push_back({ design_level_1 , {0.0f, 1.0f} });
	}

	olc::vf2d get_init_player_position(int level_id)
	{
		return levels[level_id].player_init_position;
	}

	Tile get_level_tile(olc::vi2d pos, int level_id)
	{
		if (pos.x < LEVEL_DESIGN_N_TILES_X && pos.y < LEVEL_DESIGN_N_TILES_Y && pos.x >= 0 && pos.y >= 0)
		{
			return Tile(levels[level_id].design[(pos.x + LEVEL_DESIGN_N_TILES_X * pos.y)], pos);
		}
		else
		{
			return Tile(' ', pos);
		}
	}

	void set_static_creature(char symbol, StaticCreature* static_creature)
	{
		static_creatures[symbol] = static_creature;
	}

	//void get_static_creature_tile_positions(char symbol, )

};