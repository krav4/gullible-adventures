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

void Level::clear_trashcans()
{
	trashcans.clear();
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

	m_lava_sprite = std::make_unique<olc::Sprite>(tile_spritesheets->lava);
	m_lava_decal = std::make_unique<olc::Decal>(m_lava_sprite.get());


	std::wstring design;
	design += L".......................C..................C..............C.....................C...................";
	design += L".C.........C.................C..........................................C...........C.........C....";
	design += L".....C.............C..MMMM.............C.........C............C............C.......................";
	design += L"........M...C................C.................C..MMM.........M........MM...........C......C.......";
	design += L"...M.C................T....T.................M...........T...T.....T.........C.....................";
	design += L"..................#############........................##################..........................";
	design += L"........L...............................MMM.........###..................#######......Z...........E";
	design += L"###############.................#####..........#####.....DDDDDDDDDDDDDD...........#################";
	design += L"................DDDDDDDDDDDDD............T............DDDDDDDDDDDDDDDDDDDDDD.......................";
	design += L".............DDDDDDDDDDDDDDDDDDDD....##########..DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD...................";
	design += L"...........DDDDDDDDDDDDDDDDDDDDDDDD.............DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD.................";
	design += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
	design += L"...................................................................................................";
	design += L"...................................................................................................";
	design += L"...................................................................................................";
	design += L"...................................................................................................";
	
	Level level1(design, eng, trash_spritesheets);
	level1.player_init_position = { 0.0f, 1.0f };
	levels.push_back(level1);

	design = L"";
	design += L".......................C..................C..............C.......MMMMMMMMMMM.......................";
	design += L".C.........C.................C.............................................M........C..............";
	design += L".....C.............C....................C.........C............C.....T.....M.......................";
	design += L"............C................C.................C........C........#######...M........C..............";
	design += L".....C............C.................C..........................##......M...M.C.............T......E";
	design += L".............................................................##........M...M..........#############";
	design += L".............................................T..T...T......##..........M...M........##.............";
	design += L"..................T...............MMM.....################.............M..T..T....##...............";
	design += L"..............#########....MMM.........................................############................";
	design += L"#########..............DDDDDDDDDDDDDDDDDD..........................................................";
	design += L".....................DDDDDDDDDDDDDDDDDDDDDD........................................................";
	design += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
	design += L"...................................................................................................";
	design += L"...................................................................................................";
	design += L"...................................................................................................";
	design += L"...................................................................................................";

	Level level2(design, eng, trash_spritesheets);
	level2.player_init_position = { 0.0f, 1.0f };
	levels.push_back(level2);

	design = L"";
	design += L"...........................MMMMMMMMMMMM.............................C.............C............M...";
	design += L".........................MM.........MMMMM...............C...................C...........C......M...";
	design += L".....................MMMM......T....MMMMMMMMM...................C.................C..........C.M...";
	design += L".................MMMMM.......MMMM...MMMMMMMM.......T...T...............C..............C........M...";
	design += L"MMMMMMMMMM....MMM..........MMM......MMMMMMM.......MMMMMMMMMM..................C.............C..M...";
	design += L".........MMMMM.......T...MMM........MMMMMM.......MM........DDDMMMMM....................MMMM....M...";
	design += L"..................MMMMMMMMMMM.......MMMMM.......MM..............DDDDDD.MMM.....T....MM.........M...";
	design += L"MMMMMMMMMMMMMMMMMMMMMMMMMMMMM..................MM....................DDDDDDDDDMMMDDD...........M...";
	design += L".........................MMMM....T...T...T....MMM..............................................M...";
	design += L"...........................MMMMMMMMMMMMMMMMMMMMMM..E...............T...T..T...T.T..T...........M...";
	design += L"..............................................MMMMMMMMMMMMMM...########################...MMMMMM...";
	design += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
	design += L"...................................................................................................";
	design += L"...................................................................................................";
	design += L"...................................................................................................";
	design += L"...................................................................................................";

	Level level3(design, eng, trash_spritesheets);
	level3.player_init_position = { 0.0f, 5.0f };
	levels.push_back(level3);

	design = L"";
	design += L"MMMMMMMM.................C....C.C.....C....C...C....C.....C................C............C..........";
	design += L"M.....MM...................C.....C....C....C....C...C.......C......................................";
	design += L"M.....MM........C......C.....C...C.....C....C...C.......C.....C....C...............................";
	design += L"MM....MM..............C.....C......C...C.....C.....C......C...C...............C............C.......";
	design += L"M.....MM.................C.......C...C....MM....C.....C.....C....C.................................";
	design += L"M...MMMM....C................C.........C...C.....MM.......C........................................";
	design += L"M.....MM.......................................C.......M...................C...........C...........";
	design += L"M.....MM...............................M.....C.......C.............................................";
	design += L"MMM...MM.................C.......C...............C........M......C.................................";
	design += L"M.....MM..........................................................................................E";
	design += L"M.....MM............................M........................M.....................MMMMMMMMMMMMMMMM";
	design += L"M...MMMM........................................................C..............M...................";
	design += L"M.....MM........................................................M.........M........................";
	design += L"M...............MM.....MMMM.....MM...................................M.............................";
	design += L"MMMMMMMMMMMM.......................................................................................";
	design += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";

	Level level4(design, eng, trash_spritesheets);
	level4.player_init_position = { 1.0f, 1.0f };
	levels.push_back(level4);

	design = L"";
	design += L"............................................C..........................................C...........";
	design += L"........C.....................C....................C...............C.....C.........................";
	design += L"............................................MMMMMMMMMMMMMMMMMMM............C.C......C...........MMM";
	design += L"...................C..........................................M....C..C.............T............EM";
	design += L"........C.......................C........C....................M.........C...##....####.....MMMMMMMM";
	design += L"...................C......C..........................T........M....C............C..................";
	design += L"..................C...............................#####......MM.......#.................C..........";
	design += L"..................................C..........T....MMM.......MMM....................................";
	design += L"#########.................................#####MMMM........MMM.....C...............................";
	design += L"............T...........................MMMMMMMMM.........MM..............####............C........";
	design += L".........#####.....................#####MMMMMMMM...MMMMMMM.........................................";
	design += L"..............................T.........MMMMMM....MM...............................................";
	design += L"..............#####...T....#####........MMMM.....MM...............................####.............";
	design += L"....................#####...............MMM....T.....T...T..........T.........T....................";
	design += L"........................................MMM#################.....#######...#####...................";
	design += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";

	Level level5(design, eng, trash_spritesheets);
	level5.player_init_position = { 1.0f, 6.0f };
	levels.push_back(level5);


	design = L"";
	design += L"...........................MMMMMMMMMM..............................................................";
	design += L"....................................M..................................................MMMMMMMMMMMM";
	design += L"....................................M.............................................................E";
	design += L".............................T......M...............................##...#....MMMMMMMMMMMMMMMMMMMMM";
	design += L"...............#####.....MMMMMMM....M..............................................................";
	design += L"...............................M....M.........................#....................................";
	design += L"........T......................M....M..............................................................";
	design += L".....######....................M....M..............................................................";
	design += L"...............................M....M.............................#####............................";
	design += L"...............................M....M..............................................................";
	design += L"...............####............M............................................MMMMMM.................";
	design += L"...............................M...................................................................";
	design += L".........###...................M.....................T..................T..........................";
	design += L"...............................M........T......T...MMMMM..............MMMMM........................";
	design += L"#######........................MMMMMMMMMMMMMMMMDDDDMMMMMDDDDMMMMMMDDDDMMMMMDDDDDDDDDDDDDD..........";
	design += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";

	Level level6(design, eng, trash_spritesheets);
	level6.player_init_position = { 1.0f, 9.0f };
	levels.push_back(level6);
	design = L"";
	design += L"..........................C......C..................C.....MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM";
	design += L".C......C......C.....C................C.....C.............................C.......................M";
	design += L".......................................................C................................C.........M";
	design += L".......C.......C............C..............C......................................................M";
	design += L"...........................................................###############.........###MMMMMMMMMMMMM";
	design += L"........................................................####.....................###...............";
	design += L".................A......L.......Z......................###.....................###.................";
	design += L"#######################################################......................###...................";
	design += L"...........................................................................###.....................";
	design += L".........................................................................###.......................";
	design += L".......................................................................###.........................";
	design += L"...............................T.....................................###...........................";
	design += L".....T..T...............T...MMMMMM.................................###.............................";
	design += L".....MMMMM............MMMMMMMDDDDMMMM.......T...T.....T..........#####.............................";
	design += L"MMMMMMMMMMMMMMDDDDMMMMMMMMMMDDDDDMMMMMMMMMMMMMMMMMMMMMMMDDDDDMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM";
	design += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";

	Level final_level(design, eng, trash_spritesheets);
	final_level.player_init_position = { 1.0f, 8.0f };
	levels.push_back(final_level);

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

olc::Decal* LevelDesigns::get_lava_decal()
{
	return m_lava_decal.get();
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