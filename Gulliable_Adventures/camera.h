#pragma once
#include "olcPixelGameEngine.h"
#include "level_designs.h"

class Camera
{
private:
	
	olc::vf2d f_tile_offset;
	olc::vf2d f_center_pos;
	olc::vi2d px_tile_pos;
	olc::vi2d n_visible_tiles;
	olc::vi2d delta_tile;
	olc::PixelGameEngine* eng = nullptr;
	LevelDesigns* ldesigns = nullptr;
	

public:
	Camera() 
	{
		// default center is in the middle of the screen
		f_center_pos = { (float)LEVEL_DESIGN_N_TILES_X / 2.0f, (float)LEVEL_DESIGN_N_TILES_Y / 2.0f };
		n_visible_tiles = { 20, 10 };
		// tile offset to the top left position - should be center minus dimensions in tiles divided by 2
		f_tile_offset = { (float)(f_center_pos.x - n_visible_tiles.x/2.0f), (float)(f_center_pos.y - n_visible_tiles.y/2.0f) };
		// default tile positions start at top left of the screen
		px_tile_pos = { 0 ,0 };
		// default visible tiles is 20x10 tiles
		
	};
	Camera(olc::PixelGameEngine* engine, LevelDesigns * designs)
	{
		Camera();
		eng = engine;
		ldesigns = designs;

		n_visible_tiles.x = engine->ScreenWidth() / PX_TILE_SIZE_X;
		n_visible_tiles.y = engine->ScreenHeight() / PX_TILE_SIZE_Y;
	}
	void set_center_position(olc::vf2d pos)
	{

		f_center_pos = pos;
		f_tile_offset.x = f_center_pos.x - (float)(n_visible_tiles.x) / 2;
		f_tile_offset.y = f_center_pos.y - (float)(n_visible_tiles.y) / 2;

		if (f_tile_offset.x < 0.0f) f_tile_offset.x = 0.0f;
		if (f_tile_offset.y < 0.0f) f_tile_offset.y = 0.0f;

		//if (tile_offset.x > LEVEL_DESIGN_N_TILES_X - n_visible_tiles.x) tile_offset.x = LEVEL_DESIGN_N_TILES_X - n_visible_tiles.x;
		//if (tile_offset.y > LEVEL_DESIGN_N_TILES_Y - n_visible_tiles.y) tile_offset.y = LEVEL_DESIGN_N_TILES_Y - n_visible_tiles.y;
	}

	olc::vf2d get_f_center_position()
	{
		return f_center_pos;
	}

	olc::vf2d get_f_tile_offset()
	{
		return f_tile_offset;
	}

	void draw_level_scene(int level_id, float fElapsedTime)
	{
		olc::vi2d tile_draw_dims;
		for (int x_tile = 0; x_tile < n_visible_tiles.x; x_tile++)
		{
			for (int y_tile = 0; y_tile < n_visible_tiles.y; y_tile++)
			{
				Tile tile = ldesigns->get_level_tile({ (x_tile + (int)f_tile_offset.x), (y_tile + (int)f_tile_offset.y) }, level_id);
				delta_tile.x = (f_tile_offset.x - (int)f_tile_offset.x) * tile.size.x;
				delta_tile.y = (f_tile_offset.y - (int)f_tile_offset.y) * tile.size.y;

				px_tile_pos.x = x_tile * tile.size.x - delta_tile.x;
				px_tile_pos.y = y_tile * tile.size.y - delta_tile.y;
				// drawing out static creatures
				for (auto& creature : ldesigns->static_creatures)
				{
					// if the current tile is the creature symbol, draw the creature
					if (tile.symbol == creature.first)
					{
						creature.second->set_position_px(px_tile_pos);
						creature.second->draw(fElapsedTime);
					}
				}
				// drawing out the map
				switch(tile.symbol)
				{
				case LEVEL_DESIGN_DIRT:
					eng->FillRect(px_tile_pos, tile.size, olc::DARK_GREY);
					break;

				case LEVEL_DESIGN_SKY:
					break;

				default:
					eng->FillRect(px_tile_pos, tile.size, olc::BLACK);
					break;
				}
			}
		}
	}


};