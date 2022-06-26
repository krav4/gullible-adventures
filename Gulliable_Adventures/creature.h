#pragma once

#include "olcPixelGameEngine.h"
#include "animation.h"
#include "tile.h"
#include "config.h"
#include <string.h>
#include <unordered_map>

constexpr int default_creature_hp = 30;
constexpr int default_creature_damage = 3;
//// forward declaring some classes
class LevelDesigns;
struct Level;

struct AnimatedCreatureSurroundingTiles
{
	Tile right_tile_top;
	Tile right_tile_bottom;
	Tile left_tile_top;
	Tile left_tile_bottom;
	Tile bottom_tile_left;
	Tile bottom_tile_right;
	Tile top_tile_left;
	Tile top_tile_right;
};


struct TileSpriteSheets
{
	std::string dirt;
	std::string cloud;
	std::string exit;
	std::string metal; 
};

typedef struct SpriteConfig
{
	std::string image_name;
	olc::vi2d dims;
	olc::vf2d scale;
} SpriteConfig;

struct TrashCanSpriteSheets
{
	std::string walk_right_spritesheet;
	std::string walk_left_spritesheet;
	std::string dead_spritesheet;
	int walk_tile_count;
	int walk_tile_rows;
	int walk_tile_cols;
	int px_width;
	int px_height;
};



class Creature
{
public:
	std::string name = "Creature";
	const olc::vi2d popup_text_offset = { 0, -30 };
	olc::vf2d pos = { 0.0f, 0.0f };
	olc::vi2d pos_px = { 0, 0 };
	olc::vf2d vel = { 0.0f, 0.0f };
	olc::vi2d dims = { PX_TILE_SIZE_X, PX_TILE_SIZE_Y };
	olc::vf2d scale = { 1.0f, 1.0f };

	olc::PixelGameEngine* engine = nullptr;
	Creature();
	Creature(olc::PixelGameEngine* engine_input);
	Creature(const Creature&);
	~Creature() {};

	olc::vi2d tile_to_px(olc::vf2d tiles, olc::vf2d camera_offset);

	// set float position, does not update pixel position
	void set_position(olc::vf2d position);

	// set pixel position
	void set_position_px(olc::vi2d position);

	olc::vf2d get_f_tile_position();

	olc::vi2d get_px_position(void);

	olc::vi2d get_dimensions(void);

	void set_velocity(olc::vf2d velocity);

	olc::vf2d get_velocity(void);

	virtual olc::vi2d emit_text_position(olc::vi2d extra_offset_px = { 0, 0 });
	virtual void draw(float fElapsedTime) = 0;
};


class StaticCreature : public Creature
{
public:
	// position in global level tile coordinates
	olc::vi2d level_tile_pos;
	std::unique_ptr<olc::Decal> decal;
	std::unique_ptr<olc::Sprite> sprite;
private:

	int m_interaction_id = 0;
	int m_dialogue_id = 0;
	bool m_is_interacting = false;
protected:
	std::vector<std::vector<std::string>> interactions;
public:
	StaticCreature();
	StaticCreature(olc::PixelGameEngine* engine, const SpriteConfig* config, std::string name_input);
	void increment_interaction();
	void increment_dialogue();
	int get_dialogue_id();
	int get_interaction_id();
	void reset_dialogue();
	void set_interaction_status(bool is_interacting);
	bool get_interaction_status();
	std::string get_dialogue();
	virtual void draw(float fElapsedTime) override;
};

class AnimatedCreature : public Creature
{
public:
	bool is_on_even_ground = false;
	int health_points = default_creature_hp;
	int hit_damage = default_creature_damage;
	bool is_pointing_right = true;
	bool is_walking = false;
	bool is_jumping = false;
	bool is_standing = false;
	bool is_dead = false;
protected:
	AnimatedCreatureSurroundingTiles tiles;
	float animation_interval = 0.08f;

private:
	virtual bool is_tile_solid(Tile* tile, LevelDesigns* levels);
public:
	AnimatedCreature();
	AnimatedCreature(olc::PixelGameEngine* eng);
	AnimatedCreature(const AnimatedCreature&);
	virtual void reset_health_points(int new_hp = -1);
	virtual void update_state(float fElapsedTime, olc::vf2d camera_offset) = 0;
	virtual void update_surrounding_tiles(Level* current_level);
	virtual bool check_next_to_symbol(char symbol);
	virtual bool resolve_collisions(LevelDesigns* levels, int level_id, bool resolve_ground = true, bool resolve_ceiliing = true);
};

class Trashcan : public AnimatedCreature
{
private:
	std::unique_ptr<SpriteAnimation> walk_right_animation;
	std::unique_ptr<SpriteAnimation> walk_left_animation;

	std::unique_ptr<olc::Sprite> walk_right_sprite;
	std::unique_ptr<olc::Sprite> walk_left_sprite;
	std::unique_ptr<olc::Sprite> death_sprite;

	std::unique_ptr<olc::Decal> walk_right_decal;
	std::unique_ptr<olc::Decal> walk_left_decal;
	std::unique_ptr<olc::Decal> death_decal;

	TrashCanSpriteSheets m_spritesheets;

	int pixels_patrolled = 0;
	int px_patrol_limit = 100;
	float patrol_velocity = 1.0f;
	olc::vi2d prev_pos_px;

	void create_decals();

public:
	Trashcan(olc::PixelGameEngine* eng, TrashCanSpriteSheets* spriteSheets, int patrol_limit = 100);
	Trashcan(const Trashcan& original);
	bool register_hit();
	virtual void update_state(float fElapsedTime, olc::vf2d camera_offset) override;
	virtual void draw(float fElapsedTime) override;
};


class Lupi : public StaticCreature
{

public:
	Lupi(olc::PixelGameEngine* engine, const SpriteConfig* config, std::string name_input);
};

class Lizzie : public StaticCreature
{

public:
	Lizzie(olc::PixelGameEngine* engine, const SpriteConfig* config, std::string name_input);
};

class Dad : public StaticCreature
{

public:
	Dad(olc::PixelGameEngine* engine, const SpriteConfig* config, std::string name_input);
};