#pragma once
#include "olcPixelGameEngine.h"
#include "animation.h"
#include "config.h"
#include "level.h"
#include "projectile.h"
#include <unordered_map>
constexpr float player_walk_flip_offset = 1.0f;
constexpr float player_walk_speed = 7.0f;
constexpr float player_walk_animation_interval = 0.02f;
constexpr float player_scale = 0.5f;
constexpr float player_jump_impulse_duration = 0.02f;
constexpr float player_jump_speed = -15.0f;
constexpr int player_health = 30;
constexpr float hit_draw_timer = 0.2f;
constexpr float hit_register_timer = 0.4f;
constexpr float projectile_emission_timer = 0.15f;
constexpr float hit_velocity_multiplier = 12.0f;
constexpr float player_sliding_friction = -10.0f;
constexpr float player_air_drag = -3.0f;
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
	float m_proj_emission_timer = projectile_emission_timer;
	// initially no projectile, cant use weapons
	bool m_has_projectile = false;

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
	float m_hit_draw_timer = 0.0f;
	float m_hit_register_timer = 0.0f;

	std::string proj_spritesheet;
	std::unique_ptr<Projectile>  projectile;

	std::unique_ptr<olc::Sprite> proj_sprite;
	std::unique_ptr < olc::Decal> proj_decal;

public:
	Player(olc::PixelGameEngine * engine_input, PlayerSpriteSheets * spriteSheets, std::string projectile_spritesheet) : AnimatedCreature(engine_input)
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

		proj_spritesheet = projectile_spritesheet;
		proj_sprite = std::make_unique<olc::Sprite>(proj_spritesheet);
		proj_decal = std::make_unique<olc::Decal>(proj_sprite.get());
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
			vel.x += player_air_drag * vel.x * fElapsedTime;
		}

		if (!is_walking && is_on_even_ground && !is_hit)
		{
			vel.y = 0.0f;
			// adding "sliding" drag to the player 
			vel.x += player_sliding_friction * vel.x * fElapsedTime;
			if (abs(vel.x) <= 0.02f)
			{
				vel.x = 0.0f;
			}
		}
		if (is_hit)
		{
			// calculating velocity for blowback from the enemy
			vel = hit_velocity * hit_velocity_multiplier;
			is_hit = false;
		}
		// kinematics, pffffff
		pos += vel * fElapsedTime;
		
		// update pixel position based on tiles
		pos_px = tile_to_px(pos, camera_offset);

		// decrement projectile emission timer
		if (m_proj_emission_timer >= 0.0f) m_proj_emission_timer -= fElapsedTime;
		if (m_hit_register_timer >= 0.0f) m_hit_register_timer -= fElapsedTime;
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

	char check_next_to_static_creature(LevelDesigns * levels)
	{
		char ret = ' ';
		for (auto& npc : levels->static_creatures)
		{
			// first is the character symbol
			if (check_next_to_symbol(npc.first))
			{
				ret = npc.first;
				break;
			}
		}
		return ret;
		
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

	bool check_hitbox(AnimatedCreature* creature, float fElapsedTime)
	{
		if (creature->is_dead)
		{
			// ignore hitbox if offending creature is dead
			return false;
		}
		olc::vf2d creature_pos = creature->get_f_tile_position();
		// check if enemy is next to us, and check if the timer is 0
		if ((pos.x + 1.0f > creature_pos.x && pos.x < creature_pos.x + 1.0f) &&
			(pos.y + 1.0f > creature_pos.y) && (pos.y < creature_pos.y + 1.0f) && 
			(m_hit_register_timer <= 0.0f))
		{
			// this flag is set for the update_state to take hit into account and change velocity
			// since player velocity is managed in update_state
			is_hit = true;
			hit_velocity = { pos.x - creature_pos.x, pos.y - creature_pos.y};
			if (hit_velocity.y > -0.1f)
			{
				hit_velocity.y = -0.5f;
			}
			hit_velocity = hit_velocity.norm();
			health_points -= creature->hit_damage;
			// set hit timer for drawing red tint over the decal
			m_hit_draw_timer = hit_draw_timer;
			// set timer so that enemy doesnt hit every frame
			m_hit_register_timer = hit_register_timer;
		}
		return is_hit;
	}

	void give_projectile()
	{
		m_has_projectile = true;
	}

	std::unique_ptr<Projectile> emit_projectile()
	{
		if (!m_has_projectile)
		{
			return nullptr;
		}
		
		
		if (m_proj_emission_timer >= 0)
		{
			// dont emit anything if the time is not right
			return nullptr;
		}

		olc::vf2d proj_vel;
		olc::vf2d proj_pos;
		projectile = std::make_unique<Projectile>(engine, proj_decal.get());
		proj_pos.x = pos.x;
		proj_pos.y = pos.y;
		proj_vel.y = 0.0f;
		projectile->set_position(proj_pos);
		if (is_pointing_right)
		{
			proj_vel.x += projectile_velocity;
		}
		else
		{
			proj_vel.x -= projectile_velocity;
		}
		projectile->set_velocity(proj_vel);
		m_proj_emission_timer = projectile_emission_timer;
		return std::move(projectile);
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
			if (m_hit_draw_timer > 0.0f)
			{
				engine->DrawPartialDecal(pos_px, draw_decal, adata.sourcePos, adata.sourceSize, scale, olc::RED);
				m_hit_draw_timer -= fElapsedTime;
			}
			else
			{
				engine->DrawPartialDecal(pos_px, draw_decal, adata.sourcePos, adata.sourceSize, scale);
			}
		}
	}
};

