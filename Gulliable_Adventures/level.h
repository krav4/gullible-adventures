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
	std::vector<Trashcan> trashcans;
	olc::PixelGameEngine * m_eng;
	TrashCanSpriteSheets m_trashcan_spritesheets;
	void generate_trashcan_vector();
public:
	Level();
	Level(std::wstring design, olc::PixelGameEngine* eng, TrashCanSpriteSheets* spriteSheets);
	std::vector<Tile> get_tiles_with_symbol(char symbol);
	Tile get_level_tile(olc::vi2d pos);
	std::vector<Trashcan>* get_trashcans();
	void reset_trashcans();
	void clear_trashcans();
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
	std::unique_ptr<olc::Sprite> m_metal_sprite;
	std::unique_ptr<olc::Decal> m_metal_decal;
	std::unique_ptr<olc::Sprite> m_lava_sprite;
	std::unique_ptr<olc::Decal> m_lava_decal;

	TrashCanSpriteSheets trash_sprites;
	olc::PixelGameEngine* m_eng;
public:
	std::unordered_map<char, StaticCreature*> static_creatures;
	LevelDesigns(olc::PixelGameEngine * eng, TileSpriteSheets* tile_spritesheets, TrashCanSpriteSheets* trash_spritesheets);
	
	olc::vf2d get_init_player_position(int level_id);

	olc::Decal* get_dirt_decal();

	olc::Decal* get_cloud_decal();

	olc::Decal* get_exit_decal();

	olc::Decal* get_metal_decal();

	olc::Decal* get_lava_decal();

	Level* get_level(int level_id);

	void set_static_creature(char symbol, StaticCreature* static_creature);
};

class Tree
{
	std::unique_ptr<olc::Sprite> m_sprite;
	std::unique_ptr<olc::Decal> m_decal;
	SpriteConfig m_tree_config;
public:
	Tree(SpriteConfig * tree_config);
	void draw(olc::PixelGameEngine* eng, olc::vi2d pos_px);
};