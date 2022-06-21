#pragma once
#include "olcPixelGameEngine.h"
#include "level_designs.h"

class Camera
{
private:
	
	olc::vf2d f_tile_offset;
	olc::vf2d f_center_pos;
	olc::vi2d px_display_center;
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
		
		px_display_center.x = (n_visible_tiles.x / 2) * PX_TILE_SIZE_X;
		px_display_center.y = (n_visible_tiles.y / 2) * PX_TILE_SIZE_Y;
		
	};
	Camera(olc::PixelGameEngine* engine, LevelDesigns * designs)
	{
		Camera();
		eng = engine;
		ldesigns = designs;

		n_visible_tiles.x = engine->ScreenWidth() / PX_TILE_SIZE_X;
		n_visible_tiles.y = engine->ScreenHeight() / PX_TILE_SIZE_Y;

		px_display_center.x = engine->ScreenWidth()/2;
		px_display_center.y = engine->ScreenHeight()/2;
	}
	void set_center_position(olc::vf2d pos)
	{
		f_center_pos = pos;
		// calculate the offset based on camera center position
		f_tile_offset.x = f_center_pos.x - (float)(n_visible_tiles.x) / 2;
		f_tile_offset.y = f_center_pos.y - (float)(n_visible_tiles.y) / 2;

		// check if we are at left boundary of level map
		if (f_tile_offset.x < 0.0f) f_tile_offset.x = 0.0f;
		if (f_tile_offset.y < 0.0f) f_tile_offset.y = 0.0f;
		// check if we are at right boundary of level map
		if (f_tile_offset.x > LEVEL_DESIGN_N_TILES_X - n_visible_tiles.x) f_tile_offset.x = LEVEL_DESIGN_N_TILES_X - n_visible_tiles.x;
		if (f_tile_offset.y > LEVEL_DESIGN_N_TILES_Y - n_visible_tiles.y) f_tile_offset.y = LEVEL_DESIGN_N_TILES_Y - n_visible_tiles.y;
	}

	olc::vf2d get_f_center_position()
	{
		return f_center_pos;
	}

	olc::vf2d get_f_tile_offset()
	{
		return f_tile_offset;
	}

	void draw_pop_up(const std::string pop_up_text, olc::vi2d pos_px)
	{
		eng->DrawString(pos_px, pop_up_text);
	}

	void draw_endgame()
	{
		std::string death_mes = "YOU DIED (SAD)";
		eng->DrawString({ px_display_center.x - 500, px_display_center.y - 100}, death_mes, olc::VERY_DARK_RED, 10);
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
			
				// drawing out the map
				switch(tile.symbol)
				{
				case LEVEL_DESIGN_DIRT:
					eng->FillRect(px_tile_pos, tile.size, olc::DARK_GREY);
					break;

				case LEVEL_DESIGN_SKY:
					break;

				case LEVEL_DESIGN_DEATH:
					break;

				case LEVEL_DESIGN_LUPI:
					ldesigns->static_creatures[LEVEL_DESIGN_LUPI]->set_position_px(px_tile_pos);
					ldesigns->static_creatures[LEVEL_DESIGN_LUPI]->draw(fElapsedTime);
					break;

				default:
					eng->FillRect(px_tile_pos, tile.size, olc::BLACK);
					break;
				}
			}
		}
	}


};