#pragma once
#include "olcPixelGameEngine.h"
#include "animation.h"
#include "config.h"

constexpr float player_walk_flip_offset = 1.0f;
constexpr float player_walk_speed = 9.0f;
constexpr float player_walk_animation_interval = 0.08f;
constexpr float player_scale = 0.5f;

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
	AnimationData adata;
	std::unique_ptr<SpriteAnimation> walk_right_animation;
	std::unique_ptr<SpriteAnimation> walk_left_animation;
	std::unique_ptr<olc::Sprite> walk_right_sprite;
	std::unique_ptr<olc::Sprite> walk_left_sprite;
	std::unique_ptr<olc::Decal> walk_right_decal;
	std::unique_ptr<olc::Decal> walk_left_decal;
	float animation_interval = 0.08f;
	int standing_tile = 0;
;

public:

	bool is_walking_backward = false;
	bool is_walking = false;
	bool is_standing = true;
	bool is_jumping = false;
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
		pos = velocity;
	}

	olc::vf2d get_velocity(void)
	{
		return vel;
	}

	void update_position_from_walk(float fElapsedTime, olc::vf2d camera_offset)
	{
		if (engine->GetKey(olc::Key::LEFT).bHeld)
		{
			is_walking_backward = true;
			is_standing = false;
			vel = { -walk_speed, 0 };
		}
		else if (engine->GetKey(olc::Key::RIGHT).bHeld)
		{
			is_walking_backward = false;
			is_standing = false;
			vel = { walk_speed, 0 };
		}
		else if (engine->GetKey(olc::Key::UP).bHeld)
		{
			is_jumping = true;
			is_standing = false;
			vel = { 0, -5 };
		}
		else if (engine->GetKey(olc::Key::DOWN).bHeld)
		{
			is_jumping = true;
			is_standing = false;
			vel = { 0, 5 };
		}
		else
		{
			is_standing = true;
			
			//walk_flip_offset = 0.0f;
			vel = { 0.0f, 0.0f };
		}

		pos += vel * fElapsedTime;
		

		// update pixel position based on tiles
		pos_px = tile_to_px(pos, camera_offset);
	}

	void draw_walk(float fElapsedTime)
	{
		olc::Decal* decal_ptr;
		if (is_walking_backward)
		{
			if (is_standing || is_jumping)
			{
				adata = walk_left_animation->GetSpritesheetTile(standing_tile);
			}
			else
			{
				adata = walk_left_animation->GetInfo(fElapsedTime);
			}

			engine->DrawPartialDecal(pos_px, walk_left_decal.get(), adata.sourcePos, adata.sourceSize, scale);
		}
		else
		{
			if (is_standing || is_jumping)
			{
				adata = walk_right_animation->GetSpritesheetTile(standing_tile);
			}
			else
			{
				adata = walk_right_animation->GetInfo(fElapsedTime);
			}
			engine->DrawPartialDecal(pos_px, walk_right_decal.get(), adata.sourcePos, adata.sourceSize, scale);
		}
		// draw using walk decal
		
	}
};