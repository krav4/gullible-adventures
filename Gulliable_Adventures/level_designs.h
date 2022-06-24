#pragma once
#include "olcPixelGameEngine.h"
#include "config.h"
#include "creature.h"
#include <string.h>
#include <unordered_map>

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

struct TileSpriteSheets
{
	std::string dirt;
	std::string cloud;
	std::string exit;
};

class LevelDesigns
{
private:
	std::vector<Level> levels;
	std::unique_ptr<olc::Sprite> m_dirt_sprite;
	std::unique_ptr<olc::Decal> m_dirt_decal;
	std::unique_ptr<olc::Sprite> m_cloud_sprite;
	std::unique_ptr<olc::Decal> m_cloud_decal;
	std::unique_ptr<olc::Sprite> m_exit_sprite;
	std::unique_ptr<olc::Decal> m_exit_decal;
	olc::PixelGameEngine* m_eng;
public:
	std::unordered_map<char, StaticCreature*> static_creatures;
	LevelDesigns(TileSpriteSheets* tile_spritesheets)
	{
		m_dirt_sprite = std::make_unique<olc::Sprite>(tile_spritesheets->dirt);
		m_dirt_decal = std::make_unique<olc::Decal>(m_dirt_sprite.get());

		m_cloud_sprite = std::make_unique<olc::Sprite>(tile_spritesheets->cloud);
		m_cloud_decal = std::make_unique<olc::Decal>(m_cloud_sprite.get());

		m_exit_sprite = std::make_unique<olc::Sprite>(tile_spritesheets->exit);
		m_exit_decal = std::make_unique<olc::Decal>(m_exit_sprite.get());

		std::wstring design_level_1;
		design_level_1 += L".......................C..................C..............C.........................................";
		design_level_1 += L".C.........C.................C..........................................C...........C..............";
		design_level_1 += L".....C.............C....................C.........C............C............C......................";
		design_level_1 += L"............C................C.................C....................................C..............";
		design_level_1 += L".....C..............................C..........................L.............C.....................";
		design_level_1 += L".............######....######..........................##################................C.........";
		design_level_1 += L"......E.............................................###..................#######..................E";
		design_level_1 += L"##########......................#####..........#####..............................#################";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L".....................................##########....................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";
		design_level_1 += L"...................................................................................................";

		levels.push_back({ design_level_1 , {0.0f, 1.0f} });

		std::wstring design_level_2;
		design_level_2 += L".......................C..................C..............C.........................................";
		design_level_2 += L".C.........C.................C..........................................C...........C..............";
		design_level_2 += L".....C.............C....................C.........C............C............C......................";
		design_level_2 += L"............C................C.................C....................................C.............";
		design_level_2 += L".....C..............................C..........................L.............C.....................";
		design_level_2 += L".............######....######..........................##################................C.........";
		design_level_2 += L"....................................................###..................#######...................";
		design_level_2 += L"##########......................#####..........#####..............................#################";
		design_level_2 += L"...................................................................................................";
		design_level_2 += L".....................................##########....................................................";
		design_level_2 += L"...................................................................................................";
		design_level_2 += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
		design_level_2 += L"...................................................................................................";
		design_level_2 += L"...................................................................................................";
		design_level_2 += L"...................................................................................................";
		design_level_2 += L"...................................................................................................";

		levels.push_back({ design_level_2 , {0.0f, 1.0f} });
	}

	olc::vf2d get_init_player_position(int level_id)
	{
		return levels[level_id].player_init_position;
	}

	olc::Decal* get_dirt_decal()
	{
		return m_dirt_decal.get();
	}

	olc::Decal* get_cloud_decal()
	{
		return m_cloud_decal.get();
	}

	olc::Decal* get_exit_decal()
	{
		return m_exit_decal.get();
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