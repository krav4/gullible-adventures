#pragma once
#include "olcPixelGameEngine.h"
#include "config.h"
#include "level.h"
#include <string.h>
#include <unordered_map>


Level::Level() 
{
	
};

Level::Level(std::wstring des, olc::PixelGameEngine * eng, TrashCanSpriteSheets * spriteSheets)
{
	m_eng = eng;
	m_trashcan_spritesheets = *spriteSheets;
	design = des;
	generate_trashcan_vector();
};

void Level::generate_trashcan_vector()
{
	std::vector<Tile> trashcan_tiles = get_tiles_with_symbol(LEVEL_DESIGN_TRASHCAN);
	if (trashcan_tiles.size() == 0)
	{
		return;
	}
	for (auto& trashcan_tile : trashcan_tiles)
	{
		Trashcan trashcan(m_eng, &m_trashcan_spritesheets);
		trashcan.set_position(trashcan_tile.n_pos);
		trashcans.push_back(trashcan);
	}
}


void Level::reset_trashcans()
{
	// TODO: i know this is inefficient to erase and construct vector again
	// but this only ever happens after player died
	trashcans.clear();
	generate_trashcan_vector();
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

std::vector<Trashcan>* Level::get_trashcans()
{
	return &trashcans;
}



LevelDesigns::LevelDesigns(olc::PixelGameEngine * eng, TileSpriteSheets* tile_spritesheets, TrashCanSpriteSheets * trash_spritesheets)
{
	m_dirt_sprite = std::make_unique<olc::Sprite>(tile_spritesheets->dirt);
	m_dirt_decal = std::make_unique<olc::Decal>(m_dirt_sprite.get());

	m_cloud_sprite = std::make_unique<olc::Sprite>(tile_spritesheets->cloud);
	m_cloud_decal = std::make_unique<olc::Decal>(m_cloud_sprite.get());

	m_exit_sprite = std::make_unique<olc::Sprite>(tile_spritesheets->exit);
	m_exit_decal = std::make_unique<olc::Decal>(m_exit_sprite.get());

	m_metal_sprite = std::make_unique<olc::Sprite>(tile_spritesheets->metal);
	m_metal_decal = std::make_unique<olc::Decal>(m_metal_sprite.get());

	std::wstring design1;
	design1 += L".......................C..................C..............C.........................................";
	design1 += L".C.........C.................C..........................................C...........C..............";
	design1 += L".....C.............C..MMMM.............C.........C............C............C......................";
	design1 += L"........M...C................C.................C..MMM...............................C..............";
	design1 += L"...M.C................T......................M...............................C.....................";
	design1 += L"..................#############........................##################................C.........";
	design1 += L"....A...L...............................MMM.........###..................#######......Z...........E";
	design1 += L"###############.................#####..........#####..............................#################";
	design1 += L".........................................T.........................................................";
	design1 += L".....................................##########....................................................";
	design1 += L"...................................................................................................";
	design1 += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
	design1 += L"...................................................................................................";
	design1 += L"...................................................................................................";
	design1 += L"...................................................................................................";
	design1 += L"...................................................................................................";
	
	Level level1(design1, eng, trash_spritesheets);
	level1.player_init_position = { 0.0f, 1.0f };
	levels.push_back(level1);

	
	std::wstring design2;
	design2 += L".......................C..................C..............C.......MMMMMMMMMMM.......................";
	design2 += L".C.........C.................C.............................................M........C..............";
	design2 += L".....C.............C....................C.........C............C...........M.......................";
	design2 += L"............C................C.................C........C........#######...M........C..............";
	design2 += L".....C.........T....................C..........................##......M...M.C....................E";
	design2 += L".............................................................##........M...M..........############.";
	design2 += L"................................................T...T......##..........M...M........##.............";
	design2 += L"...................................MM.....################.............M.......T..##...............";
	design2 += L"..............#########.....MM.........................................############................";
	design2 += L"#########..........................................................................................";
	design2 += L"...................................................................................................";
	design2 += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
	design2 += L"...................................................................................................";
	design2 += L"...................................................................................................";
	design2 += L"...................................................................................................";
	design2 += L"...................................................................................................";

	Level level2(design2, eng, trash_spritesheets);
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

olc::Decal* LevelDesigns::get_metal_decal()
{
	return m_metal_decal.get();
}


Level* LevelDesigns::get_level(int level_id)
{
	return &levels[level_id];
}

void LevelDesigns::set_static_creature(char symbol, StaticCreature* static_creature)
{
	static_creatures[symbol] = static_creature;
}


Tree::Tree(SpriteConfig* tree_config)
{
	m_tree_config = *tree_config;
	m_sprite = std::make_unique<olc::Sprite>(tree_config->image_name);
	m_decal = std::make_unique<olc::Decal>(m_sprite.get());
}

void Tree::draw(olc::PixelGameEngine* eng, olc::vi2d pos_px)
{
	eng->DrawDecal(pos_px, m_decal.get());
}