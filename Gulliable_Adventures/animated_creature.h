#pragma once
#include "creature.h"
#include "level_designs.h"
#include "config.h"

struct AnimatedCreatureSurroundingTiles
{
	Tile right_tile_top;
	Tile right_tile_bottom;
	Tile left_tile_top;
	Tile left_tile_bottom;
	Tile bottom_tile_left;
	Tile bottom_tile_right;
};

class AnimatedCreature : public Creature
{
public:
	bool is_on_even_ground = false;
	int health_points = 3;
	bool is_pointing_right = true;
	bool is_walking = false;
	bool is_jumping = false;
	bool is_standing = false;
	bool is_dead = false;
protected:
	AnimatedCreatureSurroundingTiles tiles;
	float animation_interval = 0.08f;
public:
	AnimatedCreature() { name = "AnimatedCreature"; };
	AnimatedCreature(olc::PixelGameEngine* eng) : Creature(eng) { name = "AnimatedCreature"; };

	virtual void update_state(float fElapsedTime, olc::vf2d camera_offset) = 0;

	virtual void update_surrounding_tiles(LevelDesigns* levels, int level_id)
	{
		tiles.right_tile_top = levels->get_level_tile({ (int)(pos.x + 1.0f), (int)(pos.y) }, level_id);
		tiles.right_tile_bottom = levels->get_level_tile({ (int)(pos.x + 1.0f), (int)(pos.y + 0.9f) }, level_id);
		tiles.left_tile_top = levels->get_level_tile({ (int)(pos.x), (int)(pos.y) }, level_id);
		tiles.left_tile_bottom = levels->get_level_tile({ (int)(pos.x), (int)(pos.y + 0.9f) }, level_id);
		tiles.bottom_tile_left = levels->get_level_tile({ (int)(pos.x), (int)(pos.y + 1.0f) }, level_id);
		tiles.bottom_tile_right = levels->get_level_tile({ (int)(pos.x + 0.9f), (int)(pos.y + 1.0f) }, level_id);
	}

	virtual bool is_tile_solid(Tile* tile, LevelDesigns* levels, int level_id)
	{
		if (tile->symbol != LEVEL_DESIGN_EMPTY && tile->symbol != LEVEL_DESIGN_CLOUD && tile->symbol != LEVEL_DESIGN_EXIT &&
			levels->static_creatures.find(tile->symbol) == levels->static_creatures.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual bool check_next_to_symbol(char symbol)
	{
		if ((tiles.left_tile_bottom.symbol == symbol) ||
			(tiles.left_tile_top.symbol == symbol) ||
			(tiles.right_tile_bottom.symbol == symbol) ||
			(tiles.right_tile_top.symbol == symbol))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual void resolve_collisions(LevelDesigns* levels, int level_id)
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
};


struct TrashCanSpriteSheets
{
	std::string walk_right_spritesheet;
	std::string walk_left_spritesheet;
	int walk_tile_count;
	int walk_tile_rows;
	int walk_tile_cols;
	int px_width;
	int px_height;
};

class Trashcan : public AnimatedCreature
{
private:
	std::unique_ptr<SpriteAnimation> walk_right_animation;
	std::unique_ptr<SpriteAnimation> walk_left_animation;
	
	std::unique_ptr<olc::Sprite> walk_right_sprite;
	std::unique_ptr<olc::Sprite> walk_left_sprite;

	std::unique_ptr<olc::Decal> walk_right_decal;
	std::unique_ptr<olc::Decal> walk_left_decal;

	int pixels_patrolled = 0;
	int px_patrol_limit = 100;
	float patrol_velocity = 0.5f;
	olc::vi2d prev_pos_px;

public:
	Trashcan(olc::PixelGameEngine* eng, TrashCanSpriteSheets* spriteSheets, int patrol_limit = 100) : AnimatedCreature(eng)
	{
		health_points = 3;
		animation_interval = 0.08f;

		px_patrol_limit = patrol_limit;

		walk_right_sprite = std::make_unique<olc::Sprite>(spriteSheets->walk_right_spritesheet);
		walk_right_decal = std::make_unique<olc::Decal>(walk_right_sprite.get());
		walk_right_animation = std::make_unique<SpriteAnimation>();
		walk_right_animation->SetParams(animation_interval, walk_right_sprite->width, walk_right_sprite->height, spriteSheets->walk_tile_cols, spriteSheets->walk_tile_rows, spriteSheets->walk_tile_count);

		walk_left_sprite = std::make_unique<olc::Sprite>(spriteSheets->walk_left_spritesheet);
		walk_left_decal = std::make_unique<olc::Decal>(walk_left_sprite.get());
		walk_left_animation = std::make_unique<SpriteAnimation>();
		walk_left_animation->SetParams(animation_interval, walk_left_sprite->width, walk_left_sprite->height, spriteSheets->walk_tile_cols, spriteSheets->walk_tile_rows, spriteSheets->walk_tile_count);

	}
	// overloading set position to include setting the "old" position on x axis
	void set_position(olc::vf2d position, olc::vf2d camera_offset)
	{
		pos = position;
		pos_px = tile_to_px(pos, camera_offset);
		prev_pos_px = pos_px;
	}

	virtual void update_state(float fElapsedTime, olc::vf2d camera_offset) override
	{
		
		// if we are patrolling right, set velocity to pos, else set to negative
		if (is_pointing_right)
		{
			vel.x = patrol_velocity;
		}
		else
		{
			vel.x = -patrol_velocity;
		}

		// add gravity if we are not on even ground
		if (!is_on_even_ground)
		{
			vel.y += 100.0f * fElapsedTime;
		}
		// calculate pixel position and how many pixels we patrolled so far
		prev_pos_px = pos_px;

		// kinematics
		pos += vel * fElapsedTime;

		
		pos_px = tile_to_px(pos, camera_offset);
		pixels_patrolled += (pos_px.x - prev_pos_px.x);
		// if we reached the patrolling limit, now flip the trashcan
		if (abs(pixels_patrolled) > px_patrol_limit)
		{
			is_pointing_right = !is_pointing_right;
			pixels_patrolled = 0;
		}
	}

	void draw(float fElapsedTime)
	{
		AnimationData adata;
		SpriteAnimation* animation;
		olc::Decal* draw_decal;

		if (is_pointing_right)
		{
			animation = walk_right_animation.get();
			draw_decal = walk_right_decal.get();
		}
		else
		{
			animation = walk_left_animation.get();
			draw_decal = walk_left_decal.get();
		}

		adata = animation->GetInfo(fElapsedTime);
		engine->DrawPartialDecal(pos_px, draw_decal, adata.sourcePos, adata.sourceSize, scale);
	}
};