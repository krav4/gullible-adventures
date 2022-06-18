#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include "animation.h"
#include "player.h"
#include "camera.h"
#include "creature.h"
#include "config.h"
#include <experimental/filesystem>

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
	std::unique_ptr<StaticCreature> lupi;
	//std::unique_ptr<StaticCreature> lizzy;
	
public:
	GullyGame()
	{
		// Name your application
		sAppName = "Gullyable_Adventures";
	}
	

public:
	bool OnUserCreate() override
	{
		if (!std::experimental::filesystem::exists("player_walk_right.png") ||
			!std::experimental::filesystem::exists("player_walk_left.png"))
		{
			throw std::invalid_argument("Player walk sprite does not exist!");
		}
		level_id = 0;

		PlayerSpriteSheets pSpriteSheets;
		
		pSpriteSheets.walk_right_spritesheet = "player_walk_right.png";
		pSpriteSheets.walk_left_spritesheet = "player_walk_left.png";
		pSpriteSheets.walk_tile_rows = 1;
		pSpriteSheets.walk_tile_count = 4;
		pSpriteSheets.walk_tile_cols = 4;
		pSpriteSheets.px_width = PX_TILE_SIZE_X;
		pSpriteSheets.px_height = PX_TILE_SIZE_Y;

		player = std::make_unique<Player>(this, &pSpriteSheets);

		SpriteConfig lupiConfig;
		lupiConfig.image_name = "lupi_flower.png";
		lupiConfig.dims = { PX_TILE_SIZE_X, PX_TILE_SIZE_Y };
		lupiConfig.scale = { 0.5, 0.5 };
		lupi = std::make_unique<StaticCreature>(this, &lupiConfig);

		levels = std::make_unique<LevelDesigns>();
		levels.get()->add_static_creature('L', lupi.get());

		player.get()->set_position({ 0.0f, 4.0f });

		camera = Camera(this, levels.get());
		camera.set_center_position(player.get()->get_f_tile_position());

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLUE);
		SetPixelMode(olc::Pixel::MASK);

		player.get()->update_state(fElapsedTime, camera.get_f_tile_offset());
		player.get()->resolve_collisions(levels.get(), level_id);
	
		camera.set_center_position(player.get()->get_f_tile_position());
		
		camera.draw_level_scene(0, fElapsedTime);
		camera.draw_pop_up("Hello World", lupi.get()->emit_text_position());
		player.get()->draw(fElapsedTime);
		
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