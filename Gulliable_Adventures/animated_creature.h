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
protected:
	AnimatedCreatureSurroundingTiles tiles;
	float animation_interval = 0.08f;
public:
	AnimatedCreature() { name = "AnimatedCreature"; };
	AnimatedCreature(olc::PixelGameEngine* eng) : Creature(eng) { name = "AnimatedCreature"; };

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