#pragma once
#include "olcPixelGameEngine.h"
#include "animation.h"
#include "config.h"
#include "level_designs.h"
#include "creature.h"
#include <unordered_map>
constexpr float player_walk_flip_offset = 1.0f;
constexpr float player_walk_speed = 6.0f;
constexpr float player_walk_animation_interval = 0.02f;
constexpr float player_scale = 0.5f;
constexpr float player_jump_impulse_duration = 0.1f;

struct PlayerSpriteSheets
{
	std::string walk_right_spritesheet;
	std::string walk_left_spritesheet;
	std::string death_spritesheet;
	int walk_tile_count;
	int walk_tile_rows;
	int walk_tile_cols;
	int px_width;
	int px_height;
};

struct PlayerSurroundingTiles
{
	Tile right_tile_top;
	Tile right_tile_bottom;
	Tile left_tile_top;
	Tile left_tile_bottom;
	Tile bottom_tile_left;
	Tile bottom_tile_right;
};

class Player : public Creature
{
private:
	float walk_flip_offset = 0.0f;
	std::unique_ptr<SpriteAnimation> walk_right_animation;
	std::unique_ptr<SpriteAnimation> walk_left_animation;
	std::unique_ptr<olc::Sprite> walk_right_sprite;
	std::unique_ptr<olc::Sprite> walk_left_sprite;
	std::unique_ptr<olc::Decal> walk_right_decal;
	std::unique_ptr<olc::Decal> walk_left_decal;
	std::unique_ptr<olc::Sprite> death_sprite;
	std::unique_ptr<olc::Decal> death_decal;

	PlayerSurroundingTiles tiles;
	float animation_interval = 0.08f;
	float jumping_impulse_remaining_time = player_jump_impulse_duration;
	int standing_tile = 0;

public:

	bool is_pointing_right = true;
	bool is_walking = false;
	bool is_jumping = false;
	bool is_on_even_ground = false;
	bool is_standing = false;
	bool is_dead = false;
	float walk_speed = player_walk_speed;

public:

	Player(olc::PixelGameEngine * engine_input, PlayerSpriteSheets * spriteSheets) : Creature(engine_input)
	{
		name = "Gully";
		scale = { player_scale, player_scale };
		dims.x = spriteSheets->px_width;
		dims.y = spriteSheets->px_height;
		walk_right_sprite = std::make_unique<olc::Sprite>(spriteSheets->walk_right_spritesheet);
		walk_right_decal = std::make_unique<olc::Decal>(walk_right_sprite.get());
		walk_right_animation = std::make_unique<SpriteAnimation>();
		walk_right_animation->SetParams(animation_interval, walk_right_sprite->width, walk_right_sprite->height, spriteSheets->walk_tile_cols, spriteSheets->walk_tile_rows, spriteSheets->walk_tile_count);

		walk_left_sprite = std::make_unique<olc::Sprite>(spriteSheets->walk_left_spritesheet);
		walk_left_decal = std::make_unique<olc::Decal>(walk_left_sprite.get());
		walk_left_animation = std::make_unique<SpriteAnimation>();
		walk_left_animation->SetParams(animation_interval, walk_left_sprite->width, walk_left_sprite->height, spriteSheets->walk_tile_cols, spriteSheets->walk_tile_rows, spriteSheets->walk_tile_count);
	
		death_sprite = std::make_unique<olc::Sprite>(spriteSheets->death_spritesheet);
		death_decal = std::make_unique<olc::Decal>(death_sprite.get());
	}
	~Player() {};

	void update_state(float fElapsedTime, olc::vf2d camera_offset)
	{
		if (is_dead)
		{
			return;
		}
		if (engine->GetKey(olc::Key::LEFT).bHeld)
		{
			is_pointing_right = false;
			is_walking = true;
			vel = { -walk_speed, 0 };
		}
		else if (engine->GetKey(olc::Key::RIGHT).bHeld)
		{
			is_pointing_right = true;
			is_walking = true;
			vel = { walk_speed, 0 };
		}
		else
		{
			is_walking = false;
			vel = { 0.0f, 0.0f };		
		}

		if (engine->GetKey(olc::Key::UP).bPressed && (is_on_even_ground))
		{
			is_jumping = true;
			// if we started jumping, we can consider ourselves off the ground
			is_on_even_ground = false;
		}
		// if we are jumping AND off the ground, need to calculate the impulse or go ballistic
		if (is_jumping && !is_on_even_ground)
		{
			// check if we are still impulsing
			if (jumping_impulse_remaining_time < 0.01f)
			{
				// if we are no longer in impulse, we should reset the impulse duration
				is_jumping = false;
				jumping_impulse_remaining_time = player_jump_impulse_duration;
			}
			else 
			{
				// we are still under the impulse, keep decrementing impulse time and 
				// make sure that velocity is going up
				vel.y = -20.0f;
				jumping_impulse_remaining_time -= fElapsedTime;
			}
		}
		// apply gravity if we are not on even ground
		if (!is_on_even_ground)
		{
			vel.y += 400.0f * fElapsedTime;
		}

		// kinematics, pffffff
		pos += vel * fElapsedTime;
		

		// update pixel position based on tiles
		pos_px = tile_to_px(pos, camera_offset);
	}

	void update_surrounding_tiles(LevelDesigns* levels, int level_id)
	{
		tiles.right_tile_top = levels->get_level_tile({ (int)(pos.x + 1.0f), (int)(pos.y) }, level_id);
		tiles.right_tile_bottom = levels->get_level_tile({ (int)(pos.x + 1.0f), (int)(pos.y + 0.9f) }, level_id);
		tiles.left_tile_top = levels->get_level_tile({ (int)(pos.x), (int)(pos.y) }, level_id);
		tiles.left_tile_bottom = levels->get_level_tile({ (int)(pos.x), (int)(pos.y + 0.9f) }, level_id);
		tiles.bottom_tile_left = levels->get_level_tile({ (int)(pos.x), (int)(pos.y + 1.0f) }, level_id);
		tiles.bottom_tile_right = levels->get_level_tile({ (int)(pos.x + 0.9f), (int)(pos.y + 1.0f) }, level_id);
	}

	bool is_tile_solid(Tile* tile, LevelDesigns * levels, int level_id)
	{
		if (tile->symbol != LEVEL_DESIGN_EMPTY && tile->symbol != LEVEL_DESIGN_CLOUD &&
			levels->static_creatures.find(tile->symbol) == levels->static_creatures.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void resolve_collisions(LevelDesigns* levels, int level_id)
	{
		// resolve collisions, making sure the tiles are not any of the static creatures
		// if they are static creatures, we should make sure we dont resolve collisions

		if ((is_tile_solid(&tiles.right_tile_top, levels, level_id) || (is_tile_solid(&tiles.right_tile_bottom, levels, level_id))))
		{
			if ((pos.x + 1.0f) > tiles.right_tile_top.n_pos.x)
			{
				pos.x = int(pos.x);
			}
		}
		if ((is_tile_solid(&tiles.left_tile_top, levels, level_id) || (is_tile_solid(&tiles.left_tile_bottom, levels, level_id))))
		{
			if ((pos.x) < tiles.left_tile_top.n_pos.x + 1.0f)
			{
				pos.x = int(pos.x + 1.0f);
			}
		}

		if ((is_tile_solid(&tiles.bottom_tile_left, levels, level_id) || (is_tile_solid(&tiles.bottom_tile_right, levels, level_id))))
		{
			// set y velocity to 0
			pos.y = int(pos.y);
			is_on_even_ground = true;
			vel.y = 0;
		}
		else
		{
			is_on_even_ground = false;
		}
	}

	bool check_death_zone()
	{
		if ((tiles.bottom_tile_left.symbol == LEVEL_DESIGN_DEATH) ||
			(tiles.bottom_tile_right.symbol == LEVEL_DESIGN_DEATH))
		{
			is_dead = true;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool check_next_to_static_creature()
	{
		if ((tiles.left_tile_bottom.symbol == LEVEL_DESIGN_LUPI) ||
			(tiles.left_tile_top.symbol == LEVEL_DESIGN_LUPI) ||
			(tiles.right_tile_bottom.symbol == LEVEL_DESIGN_LUPI) ||
			(tiles.right_tile_top.symbol == LEVEL_DESIGN_LUPI))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void draw(float fElapsedTime)
	{
		olc::Decal* decal_ptr;
		AnimationData adata;
		SpriteAnimation* animation;
		olc::Decal* draw_decal;
		if (!is_pointing_right)
		{
			animation = walk_left_animation.get();
			draw_decal = walk_left_decal.get();
		}
		else
		{
			animation = walk_right_animation.get();
			draw_decal = walk_right_decal.get();
		}

		if (!is_on_even_ground || !is_walking)
		{
			adata = animation->GetSpritesheetTile(standing_tile);
		}
		else
		{
			adata = animation->GetInfo(fElapsedTime);
		}

		if (is_dead)
		{
			engine->DrawDecal(pos_px, death_decal.get(), scale);
		}
		else
		{
			engine->DrawPartialDecal(pos_px, draw_decal, adata.sourcePos, adata.sourceSize, scale);
		}
	}
};

