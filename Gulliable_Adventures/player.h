#pragma once
#include "olcPixelGameEngine.h"
#include "animation.h"
#include "config.h"
#include "level_designs.h"

constexpr float player_walk_flip_offset = 1.0f;
constexpr float player_walk_speed = 6.0f;
constexpr float player_walk_animation_interval = 0.02f;
constexpr float player_scale = 0.5f;
constexpr float player_jump_impulse_duration = 0.1f;

struct PlayerSpriteSheets
{
	std::string walk_right_spritesheet;
	std::string walk_left_spritesheet;
	int walk_tile_count;
	int walk_tile_rows;
	int walk_tile_cols;
	int px_width;
	int px_height;
};

class Player
{
private:
	olc::vf2d pos;
	olc::vf2d pos_unflipped;
	olc::vi2d pos_px;
	olc::vf2d vel;
	olc::vf2d scale = { player_scale, player_scale };
	olc::vi2d dims;
	float walk_flip_offset = 0.0f;

	olc::PixelGameEngine* engine;
	std::unique_ptr<SpriteAnimation> walk_right_animation;
	std::unique_ptr<SpriteAnimation> walk_left_animation;
	std::unique_ptr<olc::Sprite> walk_right_sprite;
	std::unique_ptr<olc::Sprite> walk_left_sprite;
	std::unique_ptr<olc::Decal> walk_right_decal;
	std::unique_ptr<olc::Decal> walk_left_decal;
	float animation_interval = 0.08f;
	float jumping_impulse_remaining_time = player_jump_impulse_duration;
	int standing_tile = 0;
;

public:

	bool is_pointing_right = true;
	bool is_walking = false;
	bool is_jumping = false;
	bool is_on_even_ground = false;
	bool is_standing = false;
	float walk_speed = player_walk_speed;

public:

	Player(olc::PixelGameEngine * engine_input, PlayerSpriteSheets * spriteSheets)
	{
		engine = engine_input;
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
	}
	~Player() {};

	olc::vi2d tile_to_px(olc::vf2d tiles, olc::vf2d camera_offset)
	{
		olc::vi2d ret;
		ret.x = (tiles.x - camera_offset.x) * dims.x;
		ret.y = (tiles.y - camera_offset.y) * dims.y;
		return ret;
	}

	void set_position(olc::vf2d position)
	{
		pos = position;
	}

	olc::vf2d get_f_tile_position()
	{
		return pos;
	}

	olc::vi2d get_px_position(void)
	{
		return pos_px;
	}

	olc::vi2d get_dimensions(void)
	{
		return dims;
	}

	void set_velocity(olc::vf2d velocity)
	{
		vel = velocity;
	}

	olc::vf2d get_velocity(void)
	{
		return vel;
	}

	void update_state_from_inputs(float fElapsedTime, olc::vf2d camera_offset)
	{
		
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
			is_on_even_ground = false;
		}

		if (is_jumping && !is_on_even_ground)
		{
			if (jumping_impulse_remaining_time < 0.01f)
			{
				is_jumping = false;
				jumping_impulse_remaining_time = player_jump_impulse_duration;
			}
			else 
			{
				vel.y = -20.0f;
				jumping_impulse_remaining_time -= fElapsedTime;
			}
		}
		if (!is_on_even_ground)
		{
			vel.y += 600.0f * fElapsedTime;
		}

		pos += vel * fElapsedTime;
		

		// update pixel position based on tiles
		pos_px = tile_to_px(pos, camera_offset);
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

		engine->DrawPartialDecal(pos_px, draw_decal, adata.sourcePos, adata.sourceSize, scale);

	}

	void resolve_collisions(LevelDesigns * levels, int level_id)
	{
		Tile right_tile_top = levels->get_level_tile({ (int)(pos.x + 1.0f), (int)(pos.y) }, level_id);
		Tile right_tile_bottom = levels->get_level_tile({ (int)(pos.x + 1.0f), (int)(pos.y + 0.9f) }, level_id);
		Tile left_tile_top = levels->get_level_tile({ (int)(pos.x), (int)(pos.y) }, level_id);
		Tile left_tile_bottom = levels->get_level_tile({ (int)(pos.x), (int)(pos.y + 0.9f) }, level_id);
		Tile bottom_tile_left = levels->get_level_tile({ (int)(pos.x), (int)(pos.y + 1.0f) }, level_id);
		Tile bottom_tile_right = levels->get_level_tile({ (int)(pos.x + 0.9f), (int)(pos.y + 1.0f) }, level_id);

		if (right_tile_top.symbol != LEVEL_DESIGN_EMPTY || right_tile_bottom.symbol != LEVEL_DESIGN_EMPTY)
		{
			if ((pos.x + 1.0f) > right_tile_top.n_pos.x)
			{
				pos.x = int(pos.x);
			}
		}
		if (left_tile_top.symbol != LEVEL_DESIGN_EMPTY || left_tile_bottom.symbol != LEVEL_DESIGN_EMPTY)
		{
			if ((pos.x) < left_tile_top.n_pos.x + 1.0f)
			{
				pos.x = int(pos.x + 1.0f);
			}
		}
		if (bottom_tile_left.symbol != LEVEL_DESIGN_EMPTY || bottom_tile_right.symbol != LEVEL_DESIGN_EMPTY)
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
};

