#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include "animation.h"
#include "player.h"
#include "camera.h"
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
	std::string creature_dialogue;
	Camera camera;
	std::unique_ptr<Player> player;
	std::unique_ptr<LevelDesigns> levels;
	std::unique_ptr<Lupi> lupi;
	std::unique_ptr<Trashcan> trashcan_demo;

	bool is_game_started = false;

	olc::Pixel background_color;
	
public:
	GullyGame()
	{
		// Name your application
		sAppName = "Gullyable_Adventures";
	}
	

public:
	bool OnUserCreate() override
	{
		if (!std::experimental::filesystem::exists("resource/player_walk_right.png") ||
			!std::experimental::filesystem::exists("resource/player_walk_left.png") ||
			(!std::experimental::filesystem::exists("resource/player_death.png")))
		{
			throw std::invalid_argument("Player sprite does not exist!");
		}
		level_id = 0;
		background_color = olc::Pixel( 0, 0, 255);
		PlayerSpriteSheets pSpriteSheets;
		
		pSpriteSheets.walk_right_spritesheet = "resource/player_walk_right.png";
		pSpriteSheets.walk_left_spritesheet = "resource/player_walk_left.png";
		pSpriteSheets.death_spritesheet = "resource/player_death.png";
		pSpriteSheets.walk_tile_rows = 1;
		pSpriteSheets.walk_tile_count = 4;
		pSpriteSheets.walk_tile_cols = 4;
		pSpriteSheets.px_width = PX_TILE_SIZE_X;
		pSpriteSheets.px_height = PX_TILE_SIZE_Y;

		player = std::make_unique<Player>(this, &pSpriteSheets);

		SpriteConfig lupiConfig;
		lupiConfig.image_name = "resource/lupi_flower.png";
		lupiConfig.dims = { PX_TILE_SIZE_X, PX_TILE_SIZE_Y };
		lupiConfig.scale = { 0.5, 0.5 };
		lupi = std::make_unique<Lupi>(this, &lupiConfig, "Lupi");

		TrashCanSpriteSheets trashcan_spritesheets;
		trashcan_spritesheets.px_height = PX_TILE_SIZE_X;
		trashcan_spritesheets.px_width = PX_TILE_SIZE_Y;
		trashcan_spritesheets.walk_left_spritesheet = "resource/trashcan_walk_left.png";
		trashcan_spritesheets.walk_right_spritesheet = "resource/trashcan_walk_left.png";
		trashcan_spritesheets.walk_tile_cols = 4;
		trashcan_spritesheets.walk_tile_rows = 1;
		trashcan_spritesheets.walk_tile_count = 4;
		trashcan_demo = std::make_unique<Trashcan>(this, &trashcan_spritesheets);
		
		trashcan_demo.get()->set_velocity({ 0.0f, 0.0f });

		TileSpriteSheets tile_spritesheets;
		tile_spritesheets.dirt = "resource/dirt.png";
		tile_spritesheets.cloud = "resource/cloud.png";
		tile_spritesheets.exit = "resource/exit.png";

		levels = std::make_unique<LevelDesigns>(this, &tile_spritesheets, &trashcan_spritesheets);
		levels.get()->set_static_creature('L', lupi.get());

		player.get()->set_position({ 0.0f, 1.0f });
		player.get()->set_velocity({ 0.0f, 0.0f });

		camera = Camera(this, levels.get());
		camera.set_center_position(player.get()->get_f_tile_position());
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(background_color);
		SetPixelMode(olc::Pixel::MASK);
		Level* current_level = levels.get()->get_level(level_id);

		/** ------------------------- WELCOME SCREEN ----------------------*/
		if (!is_game_started)
		{
			if (GetKey(olc::Key::E).bPressed)
			{
				is_game_started = true;
			}
			else
			{
				DrawString({ 100 , 100 }, "Welcome to the GULLIABLE ADVENTURES!!!", olc::WHITE, 4);
				DrawString({ 100 , 150 }, "Use Arrow Keys to move, E to interact!", olc::WHITE, 4);
				DrawString({ 100 , 200 }, "Keep pressing E to exhaust dialogue!", olc::WHITE, 4);
				DrawString({ 100 , 250 }, "Press UP to jump!", olc::WHITE, 4);
				DrawString({ 100 , 300 }, "Press SPACE to attack!", olc::WHITE, 4);
				DrawString({ 100 , 400 }, "Press E to continue...", olc::WHITE, 4);
				player.get()->draw(fElapsedTime);
			}
			return true;
		}

		/*--------------------------- DRAWING THE LEVEL -----------------------*/
		// snap camera to player position
		camera.set_center_position(player.get()->get_f_tile_position());
		// draw all the level tiles
		camera.draw_level_scene(level_id, fElapsedTime);
		camera.draw_health_bar(player.get());

		/*--------------------------- UPDATING PLAYER STATE -----------------------*/
		// update player state to new
		player.get()->update_state(fElapsedTime, camera.get_f_tile_offset());
		player.get()->update_surrounding_tiles(current_level);
		player.get()->resolve_collisions(levels.get(), level_id);

		/*--------------------------- FALLING TO DEATH ---------------------*/
		if (player.get()->check_death_zone())
		{
			camera.draw_endgame();
			if (GetKey(olc::Key::E).bPressed)
			{
				player.get()->set_position(levels.get()->get_init_player_position(level_id));
				player.get()->set_velocity({ 0.0f, 0.0f });
				player.get()->is_dead = false;
			}
		}
		/*--------------------------- NPC INTERACTION  -----------------------*/
		else if (player.get()->check_next_to_static_creature())
		{
			if (!lupi.get()->get_interaction_status())
			{
				camera.draw_pop_up("press E to talk", lupi.get()->emit_text_position({ -100, -10 }));
			}
			if (GetKey(olc::Key::E).bPressed)
			{	
				// set the dialogue to whatever lupi has to say
				creature_dialogue = lupi.get()->get_dialogue();
			}
			camera.draw_pop_up(creature_dialogue, lupi.get()->emit_text_position({ -100, -10 }));
		}
		/*--------------------------- PROCEEDING TO NEXT LEVEL -----------------------*/
		else if (player.get()->check_next_to_exit())
		{
			camera.draw_pop_up("Press E for Next Level", player.get()->emit_text_position());
			if (GetKey(olc::Key::E).bPressed)
			{
				level_id++;
				player.get()->set_position(levels.get()->get_init_player_position(level_id));
			}
		}

		/*--------------------------- DRAWING ENEMIES -----------------------*/

		
		std::vector<Trashcan> * trashcans = current_level->get_trashcans();
		for (auto& trashcan : *trashcans)
		{
			trashcan.update_state(fElapsedTime, camera.get_f_tile_offset());
			trashcan.update_surrounding_tiles(current_level);
			trashcan.resolve_collisions(levels.get(), level_id);
			trashcan.draw(fElapsedTime);
			if (player.get()->check_hitbox(&trashcan))
			{
				DrawString(player.get()->get_px_position(), "ENEMY NEARBY");
			}
		}
		
		/*--------------------------- DRAWING THE PLAYER -----------------------*/
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