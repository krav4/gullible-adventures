#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include "animation.h"
#include "player.h"
#include "camera.h"
#include "config.h"

// Override base class with your custom functionality
class GullyGame : public olc::PixelGameEngine
{
private:
	int n_level_width = LEVEL_DESIGN_N_TILES_X;
	int n_level_height = LEVEL_DESIGN_N_TILES_Y;
	int tile_dim_x = PX_TILE_SIZE_X;
	int tile_dim_y = PX_TILE_SIZE_Y;
	int level_id;

	Camera camera;
	std::unique_ptr<Player> player;
	std::unique_ptr<LevelDesigns> levels;
	
public:
	GullyGame()
	{
		// Name your application
		sAppName = "Gullyable_Adventures";
	}
	

public:
	bool OnUserCreate() override
	{
		level_id = 0;

		PlayerSpriteSheets pSpriteSheets;
		pSpriteSheets.walk_right_spritesheet = "player_walk_200.png";
		pSpriteSheets.walk_left_spritesheet = "player_walk_left_200.png";
		pSpriteSheets.walk_tile_rows = 1;
		pSpriteSheets.walk_tile_count = 4;
		pSpriteSheets.walk_tile_cols = 4;
		pSpriteSheets.px_width = PX_TILE_SIZE_X;
		pSpriteSheets.px_height = PX_TILE_SIZE_Y;

		player = std::make_unique<Player>(this, &pSpriteSheets);
		levels = std::make_unique<LevelDesigns>();

		player.get()->set_position({ 0.0f, 6.0f });

		camera = Camera(this, levels.get());
		camera.set_center_position(player.get()->get_f_tile_position());

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLUE);
		SetPixelMode(olc::Pixel::MASK);
		olc::vf2d f_player_pos;
		
		player.get()->update_position_from_walk(fElapsedTime, camera.get_f_tile_offset());
		
		f_player_pos = player.get()->get_f_tile_position();

		Tile right_tile_top = levels.get()->get_level_tile({(int)(f_player_pos.x + 1.0f), (int)(f_player_pos.y)}, level_id);
		Tile right_tile_bottom = levels.get()->get_level_tile({ (int)(f_player_pos.x + 1.0f), (int)(f_player_pos.y + 0.9f) }, level_id);
		Tile left_tile_top = levels.get()->get_level_tile({ (int)(f_player_pos.x), (int)(f_player_pos.y) }, level_id);
		Tile left_tile_bottom = levels.get()->get_level_tile({ (int)(f_player_pos.x), (int)(f_player_pos.y + 0.9f) }, level_id);
		if (right_tile_top.symbol != LEVEL_DESIGN_EMPTY)
		{
			if ((f_player_pos.x + 1.0f) > right_tile_top.n_pos.x)
			{
				f_player_pos.x = int(f_player_pos.x);
				player.get()->set_position(f_player_pos);
			}
		}
		else if (right_tile_bottom.symbol != LEVEL_DESIGN_EMPTY)
		{
			if ((f_player_pos.x + 1.0f) > right_tile_bottom.n_pos.x)
			{
				f_player_pos.x = int(f_player_pos.x);
				player.get()->set_position(f_player_pos);
			}
		}
		else if (left_tile_top.symbol != LEVEL_DESIGN_EMPTY)
		{
			if ((f_player_pos.x) < left_tile_top.n_pos.x)
			{
				f_player_pos.x = int(f_player_pos.x + 1.0f) ;
				player.get()->set_position(f_player_pos);
			}
		}
		else if (left_tile_bottom.symbol != LEVEL_DESIGN_EMPTY)
		{
			if ((f_player_pos.x) < left_tile_bottom.n_pos.x)
			{
				f_player_pos.x = int(f_player_pos.x + 1.0f);
				player.get()->set_position(f_player_pos);
			}
		}
		
		player.get()->draw_walk(fElapsedTime);
		camera.set_center_position(player.get()->get_f_tile_position());
		camera.draw_level_scene(0, fElapsedTime); 
		
		return true;
	}
};

int main()
{
	GullyGame game;
	if (game.Construct(PX_WINDOW_SIZE_X, PX_WINDOW_SIZE_Y, PX_TILE_SIZE_X, PX_TILE_SIZE_Y))
		game.Start();
	return 0;
}