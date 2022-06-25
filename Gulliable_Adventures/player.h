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
constexpr float player_jump_impulse_duration = 0.08f;
constexpr float player_jump_speed = -22.0f;
constexpr int player_health = 50;
constexpr float hit_draw_timer = 0.2f;

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
	bool is_hit = false;

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
	olc::vf2d hit_velocity = {0.0f, 0.0f};
	float hit_timer = 0.0f;

public:
	Player(olc::PixelGameEngine * engine_input, PlayerSpriteSheets * spriteSheets) : AnimatedCreature(engine_input)
	{
		name = "Gully";
		health_points = player_health;
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
		if (engine->GetKey(olc::Key::LEFT).bHeld && !is_hit)
		{
			is_pointing_right = false;
			is_walking = true;
			vel = { -walk_speed, vel.y };
		}
		else if (engine->GetKey(olc::Key::RIGHT).bHeld && !is_hit)
		{
			is_pointing_right = true;
			is_walking = true;
			vel = { walk_speed, vel.y };
		}
		else
		{
			is_walking = false;	
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
				vel.y = player_jump_speed;
				jumping_impulse_remaining_time -= fElapsedTime;
			}
		}
		// apply gravity if we are not on even ground
		if (!is_on_even_ground)
		{
			vel.y += gravity * fElapsedTime;
		}

		if (!is_walking && is_on_even_ground && !is_hit)
		{
			vel = { 0.0f, 0.0f };
		}
		if (is_hit)
		{
			// calculating velocity for blowback from the enemy
			health_points--;
			vel = hit_velocity * 10.0f;
			is_hit = false;
		}
		// kinematics, pffffff
		pos += vel * fElapsedTime;
		

		// update pixel position based on tiles
		pos_px = tile_to_px(pos, camera_offset);
	}

	bool check_death()
	{
		if ((tiles.bottom_tile_left.symbol == LEVEL_DESIGN_DEATH) ||
			(tiles.bottom_tile_right.symbol == LEVEL_DESIGN_DEATH) ||
			(health_points <= 0))
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

	bool check_hitbox(AnimatedCreature* creature)
	{
		olc::vf2d creature_pos = creature->get_f_tile_position();
		if ((pos.x + 1.1f > creature_pos.x && pos.x < creature_pos.x + 1.1f) &&
			(pos.y + 1.0f > creature_pos.y))
		{

			is_hit = true;
			hit_velocity = { pos.x - creature_pos.x, pos.y - creature_pos.y};
			if (hit_velocity.y > -0.1f)
			{
				hit_velocity.y = -0.5f;
			}
			hit_velocity = hit_velocity.norm();
			// set hit timer for drawing red tint over the decal
			hit_timer = hit_draw_timer;
		}
		return is_hit;
	}

	virtual void reset_health_points(int new_hp = -1) override
	{
		if (new_hp > 0)
		{
			health_points = new_hp;
		}
		else
		{
			health_points = player_health;
		}
	}

	virtual void draw(float fElapsedTime) override
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
			if (hit_timer > 0.0f)
			{
				engine->DrawPartialDecal(pos_px, draw_decal, adata.sourcePos, adata.sourceSize, scale, olc::RED);
				hit_timer -= fElapsedTime;
			}
			else
			{
				engine->DrawPartialDecal(pos_px, draw_decal, adata.sourcePos, adata.sourceSize, scale);
			}
		}
	}
};

