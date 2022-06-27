#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include "animation.h"
#include "player.h"
#include "camera.h"
#include "config.h"
#include <experimental/filesystem>
#include "windows.h"


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
	std::unique_ptr<Lizzie> lizzie;
	std::unique_ptr<Dad> dad;
	std::unique_ptr<Tree> tree;
	std::unique_ptr<Trashcan> trashcan_demo;

	bool is_game_started = false;

	std::vector<std::unique_ptr<Projectile>> projectiles;

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
		level_id = 4;
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

		TrashCanSpriteSheets trashcan_spritesheets;
		trashcan_spritesheets.px_height = PX_TILE_SIZE_X;
		trashcan_spritesheets.px_width = PX_TILE_SIZE_Y;
		trashcan_spritesheets.walk_left_spritesheet = "resource/trashcan_walk_left.png";
		trashcan_spritesheets.walk_right_spritesheet = "resource/trashcan_walk_right.png";
		trashcan_spritesheets.dead_spritesheet = "resource/trashcan_dead.png";
		trashcan_spritesheets.walk_tile_cols = 2;
		trashcan_spritesheets.walk_tile_rows = 1;
		trashcan_spritesheets.walk_tile_count = 2;

		TileSpriteSheets tile_spritesheets;
		tile_spritesheets.dirt = "resource/dirt.png";
		tile_spritesheets.cloud = "resource/cloud.png";
		tile_spritesheets.exit = "resource/exit.png";
		tile_spritesheets.metal = "resource/metal_tile.png";
		tile_spritesheets.lava = "resource/lava.png";

		SpriteConfig lupiConfig;
		lupiConfig.image_name = "resource/lupi_flower.png";
		lupiConfig.dims = { PX_TILE_SIZE_X, PX_TILE_SIZE_Y };
		lupiConfig.scale = { 0.5, 0.5 };

		SpriteConfig lizzieConfig;
		lizzieConfig.image_name = "resource/lizzie.png";
		lizzieConfig.dims = { PX_TILE_SIZE_X, PX_TILE_SIZE_Y };
		lizzieConfig.scale = { 0.5, 0.5 };

		SpriteConfig dadConfig;
		dadConfig.image_name = "resource/dad.png";
		dadConfig.dims = { PX_TILE_SIZE_X, PX_TILE_SIZE_Y };
		dadConfig.scale = { 0.5, 0.5 };

		SpriteConfig treeConfig;
		treeConfig.image_name = "resource/tree.png";
		treeConfig.dims = { 400, 600 };
		treeConfig.scale = { 1, 1 };

		std::string projectile_spritesheet = "resource/projectile.png";

		player = std::make_unique<Player>(this, &pSpriteSheets, projectile_spritesheet);

		lupi = std::make_unique<Lupi>(this, &lupiConfig, "Lupi");
		lizzie = std::make_unique<Lizzie>(this, &lizzieConfig, "Lizzie");
		dad = std::make_unique<Dad>(this, &dadConfig, "Dad");
		tree = std::make_unique<Tree>(&treeConfig);

		levels = std::make_unique<LevelDesigns>(this, &tile_spritesheets, &trashcan_spritesheets);
		levels.get()->set_static_creature('L', lupi.get());
		levels.get()->set_static_creature('Z', lizzie.get());
		levels.get()->set_static_creature('A', dad.get());

		player.get()->set_position(levels.get()->get_init_player_position(level_id));
		player.get()->set_velocity({ 0.0f, 0.0f });

		camera = Camera(this, levels.get());
		camera.set_center_position(player.get()->get_f_tile_position());
		player.get()->give_projectile();
		//PlaySound(TEXT("resource/soundtrack_gullible.wav"), NULL, SND_LOOP | SND_ASYNC);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		/** ------------------------- FRAME INITIALIZATION ----------------------*/
		Clear(background_color);
		SetPixelMode(olc::Pixel::MASK);
		Level* current_level = levels.get()->get_level(level_id);

		// get the vector of pointers to all enemies in the level
		std::vector<Trashcan>* trashcans = current_level->get_trashcans();

		// initializing currently interacting creature to nullptr, later we check
		// if we are next to any of them and this will get assigned
		StaticCreature* interacting_creature = nullptr;
		// check if we are next to nearby creature, returns symbol from config.h
		char nearby_creature = player.get()->check_next_to_static_creature(levels.get());

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
				DrawString({ 100 , 300 }, "Avoid TRASHCANS (initially)", olc::WHITE, 4);
				DrawString({ 100 , 350 }, "Lava is DEADLY", olc::WHITE, 4);
				DrawString({ 100 , 400 }, "If you die, press E to restart level", olc::WHITE, 4);
				DrawString({ 100 , 500 }, "Press E to continue...", olc::WHITE, 4);
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
		if (player.get()->check_death())
		{
			camera.draw_endgame();
			if (GetKey(olc::Key::E).bPressed)
			{
				player.get()->set_position(levels.get()->get_init_player_position(level_id));
				player.get()->set_velocity({ 0.0f, 0.0f });
				player.get()->is_dead = false;
				player.get()->reset_health_points();
				current_level->reset_trashcans();
				// TODO: deduplicate
				trashcans = current_level->get_trashcans();
			}
		}


		/*--------------------------- UPDATING/DRAWING TRASHCANS, CHECKING HITBOX -----------------------*/

		for (auto& trashcan : *trashcans)
		{
			trashcan.update_state(fElapsedTime, camera.get_f_tile_offset());
			trashcan.update_surrounding_tiles(current_level);
			trashcan.resolve_collisions(levels.get(), level_id);
		}
		/*--------------------------- NPC INTERACTION  -----------------------*/
		
		// TODO: switch would be better
		if (nearby_creature == LEVEL_DESIGN_LUPI)
		{
			interacting_creature = lupi.get();
		}
		else if (nearby_creature == LEVEL_DESIGN_LIZZIE)
		{
			interacting_creature = lizzie.get();
		}
		else if (nearby_creature == LEVEL_DESIGN_DAD)
		{
			interacting_creature = dad.get();
		}
		else
		{
			// reset creature dialogue
			creature_dialogue = "";
			// set all static creature interactions to false
			// reset the dialogue ids
			for (auto& npc : levels.get()->static_creatures)
			{
				npc.second->reset_dialogue();
				npc.second->set_interaction_status(false);
			}
		}
		
		if (interacting_creature != nullptr)
		{
			if (!interacting_creature->get_interaction_status())
			{
				camera.draw_pop_up("press E to talk", interacting_creature->emit_text_position({ -100, -10 }));
			}
			if (GetKey(olc::Key::E).bPressed)
			{	
				interacting_creature->set_interaction_status(true);
				// set the dialogue to whatever lupi has to say
				creature_dialogue = interacting_creature->get_dialogue();
				// lizzie has special interaction - we get a weapon at the end of her dialogue
				if (interacting_creature->name == "Lizzie")
				{
					if (lizzie.get()->is_giving_weapon_to_player() && player.get()->m_has_projectile == false)
					{
						player.get()->give_projectile();
					}
				}
			}
			camera.draw_pop_up(creature_dialogue, player->emit_text_position({ -50, -40 }));
		}
		
		/*--------------------------- EMITTING PROJECTILES -----------------------*/
		if (GetKey(olc::Key::SPACE).bPressed)
		{
			std::unique_ptr<Projectile> proj = player.get()->emit_projectile();
			// will return nullptr if time is not right or player doesnt have it
			if (proj != nullptr)
			{
				// add the unique ptr to the vector of projectiles
				projectiles.push_back(std::move(proj));
			}
		}
		/*--------------------------- RESOLVING/MANAGING PROJECTILES (GOD ITS HARD) -----------------------*/
		// should have maybe used a separate class of projectiles to handle this
		// if we have any projectiles, we need to draw them or get rid of them to conserve memory
		if (projectiles.size() > 0)
		{
			olc::vf2d proj_pos;
			auto proj = projectiles.begin();
			// iterating over the vector of shared pointers to the projectiles emitted by player
			while (proj != projectiles.end())
			{
				proj_pos = proj->get()->get_f_tile_position();
				proj->get()->update_surrounding_tiles(current_level);
				// resolving collisions or getting out of bounds of the map results in deletion of the element
				// also, if we hit the enemy already with the projectile, we delete it
				// make sure we dont resolve collision with ground for the projectiles
				if (proj->get()->resolve_collisions(levels.get(), level_id, false, false) || 
					(int)proj_pos.x > LEVEL_DESIGN_N_TILES_X || 
					(int)proj_pos.x < 0 || 
					proj->get()->is_enemy_hit)
				{
					proj = projectiles.erase(proj);
				}
				// otherwise, update the projectile state, and draw
				else {
					proj->get()->update_state(fElapsedTime, camera.get_f_tile_offset());
					//check if we hit any trashcans
					for (auto& trashcan : *trashcans)
					{
						if (proj->get()->check_hit_enemy(&trashcan))
						{
							// this decrements health and marks death status if health  is 0
							trashcan.register_hit();
						}
					}
					proj->get()->draw(fElapsedTime);
					// only increment iterator if we didnt delete
					++proj;
				}
			}
		}

		/*--------------------------- PROCEEDING TO NEXT LEVEL -----------------------*/
		if (player.get()->check_next_to_exit())
		{
			camera.draw_pop_up("Press E for Next Level", player.get()->emit_text_position());
			if (GetKey(olc::Key::E).bPressed)
			{
				level_id++;
				player.get()->set_position(levels.get()->get_init_player_position(level_id));
				player.get()->reset_health_points();
				current_level = levels.get()->get_level(level_id);
				// make sure trashcans get cleared and new ones are made
				current_level->reset_trashcans();
			}
		}
		/*--------------------------- DRAWING THE TRASHCANS -----------------------*/
		for (auto& trashcan : *trashcans)
		{

			trashcan.draw(fElapsedTime);
			player.get()->check_hitbox(&trashcan, fElapsedTime);
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