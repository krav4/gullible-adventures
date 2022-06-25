#pragma once
#include "config.h"
#include "olcPixelGameEngine.h"
#include "tile.h"
#include <unordered_map>
#include "creature.h"

struct Level
{
public:
	std::wstring design;
	olc::vf2d player_init_position;

private:
	//std::vector<std::unique_ptr<Trashcan>> trashcans;
public:
	Level();
	std::vector<Tile> get_tiles_with_symbol(char symbol);
	Tile get_level_tile(olc::vi2d pos);
	//std::vector<std::unique_ptr<Trashcan>> spawn_trashcans(olc::PixelGameEngine* eng, TrashCanSpriteSheets* spriteSheets);
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
	LevelDesigns(TileSpriteSheets* tile_spritesheets);

	olc::vf2d get_init_player_position(int level_id);

	olc::Decal* get_dirt_decal();

	olc::Decal* get_cloud_decal();

	olc::Decal* get_exit_decal();

	Level* get_level(int level_id);

	void set_static_creature(char symbol, StaticCreature* static_creature);
};