#pragma once
#include "olcPixelGameEngine.h"
#include "config.h"
#include "creature.h"
//#include "animated_creature.h"
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
public:
	std::wstring design;
	olc::vf2d player_init_position;

private:
	//std::unique_ptr<std::vector<Trashcan>> trashcans;
public:
	Level()
	{
		//trashcans = std::make_unique<std::vector<Trashcan>>();
	}
	std::vector<Tile> get_tiles_with_symbol(char symbol)
	{
		std::vector<Tile> tiles_with_symbol;
		for (int xtile = 0; xtile < LEVEL_DESIGN_N_TILES_X; xtile++)
		{
			for (int ytile = 0; ytile < LEVEL_DESIGN_N_TILES_Y; ytile++)
			{
				Tile current_tile = Tile(design[(xtile + LEVEL_DESIGN_N_TILES_X * ytile)], { xtile, ytile });
				if (current_tile.symbol == symbol)
				{
					tiles_with_symbol.push_back(current_tile);
				}
			}
		}
		return tiles_with_symbol;
	}

	Tile get_level_tile(olc::vi2d pos)
	{
		if (pos.x < LEVEL_DESIGN_N_TILES_X && pos.y < LEVEL_DESIGN_N_TILES_Y && pos.x >= 0 && pos.y >= 0)
		{
			return Tile(design[(pos.x + LEVEL_DESIGN_N_TILES_X * pos.y)], pos);
		}
		else
		{
			return Tile(' ', pos);
		}
	}

	//std::unique_ptr<std::vector<Trashcan>> spawn_trashcans(olc::PixelGameEngine * eng, TrashCanSpriteSheets * spriteSheets)
	//{
	//	std::vector<Tile> trashcan_tiles = get_tiles_with_symbol(LEVEL_DESIGN_TRASHCAN);
	//	for (auto& trashcan_tile : trashcan_tiles)
	//	{
	//		Trashcan trashcan_spawn = Trashcan(eng, spriteSheets);
	//		trashcans.get()->push_back(trashcan_spawn);
	//	}
	//	// give the consumer the rights to trashcans
	//	return std::move(trashcans);
	//}
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

		Level level1;
		level1.design += L".......................C..................C..............C.........................................";
		level1.design += L".C.........C.................C..........................................C...........C..............";
		level1.design += L".....C.............C....................C.........C............C............C......................";
		level1.design += L"............C................C.................C....................................C..............";
		level1.design += L".....C..............................C..........................L.............C.....................";
		level1.design += L".............######....######..........................##################................C.........";
		level1.design += L"......T.............................................###..................#######..................E";
		level1.design += L"##########......................#####..........#####..............................#################";
		level1.design += L"...................................................................................................";
		level1.design += L".....................................##########....................................................";
		level1.design += L"...................................................................................................";
		level1.design += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
		level1.design += L"...................................................................................................";
		level1.design += L"...................................................................................................";
		level1.design += L"...................................................................................................";
		level1.design += L"...................................................................................................";

		level1.player_init_position = { 0.0f, 1.0f };
		levels.push_back(level1);

		Level level2;
		level2.design += L".......................C..................C..............C.........................................";
		level2.design += L".C.........C.................C..........................................C...........C..............";
		level2.design += L".....C.............C....................C.........C............C............C......................";
		level2.design += L"............C................C.................C....................................C.............";
		level2.design += L".....C..............................C..........................L.............C.....................";
		level2.design += L".............######....######..........................##################................C.........";
		level2.design += L"....................................................###..................#######...................";
		level2.design += L"##########......................#####..........#####..............................#################";
		level2.design += L"...................................................................................................";
		level2.design += L".....................................##########....................................................";
		level2.design += L"...................................................................................................";
		level2.design += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
		level2.design += L"...................................................................................................";
		level2.design += L"...................................................................................................";
		level2.design += L"...................................................................................................";
		level2.design += L"...................................................................................................";

		level2.player_init_position = { 0.0f, 1.0f };
		levels.push_back(level2);

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

	Level* get_level(int level_id)
	{
		return &levels[level_id];
	}

	void set_static_creature(char symbol, StaticCreature* static_creature)
	{
		static_creatures[symbol] = static_creature;
	}

	//void get_static_creature_tile_positions(char symbol, )

};