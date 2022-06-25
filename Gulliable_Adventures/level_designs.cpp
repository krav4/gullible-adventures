#pragma once
#include "olcPixelGameEngine.h"
#include "config.h"
#include "creature.h"
#include "level_designs.h"
#include <string.h>
#include <unordered_map>


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


Level::Level()
{
	//trashcans = std::make_unique<std::vector<Trashcan>>();
}
std::vector<Tile> Level::get_tiles_with_symbol(char symbol)
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

Tile Level::get_level_tile(olc::vi2d pos)
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

//std::vector<std::unique_ptr<Trashcan>> Level::spawn_trashcans(olc::PixelGameEngine* eng, TrashCanSpriteSheets* spriteSheets)
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



LevelDesigns::LevelDesigns(TileSpriteSheets* tile_spritesheets)
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

olc::vf2d LevelDesigns::get_init_player_position(int level_id)
{
	return levels[level_id].player_init_position;
}

olc::Decal* LevelDesigns::get_dirt_decal()
{
	return m_dirt_decal.get();
}

olc::Decal* LevelDesigns::get_cloud_decal()
{
	return m_cloud_decal.get();
}

olc::Decal* LevelDesigns::get_exit_decal()
{
	return m_exit_decal.get();
}

Level* LevelDesigns::get_level(int level_id)
{
	return &levels[level_id];
}

void LevelDesigns::set_static_creature(char symbol, StaticCreature* static_creature)
{
	static_creatures[symbol] = static_creature;
}