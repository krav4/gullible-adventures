#pragma once
#include "olcPixelGameEngine.h"
#include "animation.h"
#include "config.h"
#include "level.h"
#include <unordered_map>
constexpr float player_walk_flip_offset = 1.0f;
constexpr float player_walk_speed = 6.0f;
constexpr float player_walk_animation_interval = 0.02f;
constexpr float player_scale = 0.5f;
constexpr float player_jump_impulse_duration = 0.2f;

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

class Player : public AnimatedCreature
{

public:
	
	float walk_speed = player_walk_speed;

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
	float jumping_impulse_remaining_time = player_jump_impulse_duration;
	int standing_tile = 0;

public:
	Player(olc::PixelGameEngine * engine_input, PlayerSpriteSheets * spriteSheets) : AnimatedCreature(engine_input)
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

	virtual void update_state(float fElapsedTime, olc::vf2d camera_offset) override
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
				vel.y = -25.0f;
				jumping_impulse_remaining_time -= fElapsedTime;
			}
		}
		// apply gravity if we are not on even ground
		if (!is_on_even_ground)
		{
			vel.y += gravity * fElapsedTime;
		}

		// kinematics, pffffff
		pos += vel * fElapsedTime;
		

		// update pixel position based on tiles
		pos_px = tile_to_px(pos, camera_offset);
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
		if (check_next_to_symbol(LEVEL_DESIGN_LUPI))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool check_next_to_exit()
	{
		if (check_next_to_symbol(LEVEL_DESIGN_EXIT))
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

