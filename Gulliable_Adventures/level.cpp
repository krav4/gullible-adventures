#pragma once
#include "olcPixelGameEngine.h"
#include "config.h"
#include "level.h"
#include <string.h>
#include <unordered_map>

Tile::Tile()
{
	symbol = ' ';
	n_pos = { 0, 0 };
};

Tile::Tile(char tile_symbol, olc::vi2d tile_pos)
{
	symbol = tile_symbol;
	n_pos = tile_pos;
}


Level::Level()
{
	//trashcans = std::make_unique<std::vector<Trashcan>>();
}
std::vector<Tile> Level::get_tiles_with_symbol(char symbol)
{
	std::vector<Tile> tiles_with_symbol;
	for (int xtile = 0; xtile < LEVEL_DESIGN_N_TILES_X; xtile++)
	{
		for (int ytile = 0; ytile < LEVEL_DESIGN_N_TILES_Y; ytile++)
		{
			Tile current_tile = Tile(design[(xtile + LEVEL_DESIGN_N_TILES_X * ytile)], { xtile, ytile });
			if (current_tile.symbol == symbol)
			{
				tiles_with_symbol.push_back(current_tile);
			}
		}
	}
	return tiles_with_symbol;
}

Tile Level::get_level_tile(olc::vi2d pos)
{
	if (pos.x < LEVEL_DESIGN_N_TILES_X && pos.y < LEVEL_DESIGN_N_TILES_Y && pos.x >= 0 && pos.y >= 0)
	{
		return Tile(design[(pos.x + LEVEL_DESIGN_N_TILES_X * pos.y)], pos);
	}
	else
	{
		return Tile(' ', pos);
	}
}

//std::vector<std::unique_ptr<Trashcan>> Level::spawn_trashcans(olc::PixelGameEngine* eng, TrashCanSpriteSheets* spriteSheets)
//{
//	std::vector<Tile> trashcan_tiles = get_tiles_with_symbol(LEVEL_DESIGN_TRASHCAN);
//	for (auto& trashcan_tile : trashcan_tiles)
//	{
//		Trashcan trashcan_spawn = Trashcan(eng, spriteSheets);
//		trashcans.get()->push_back(trashcan_spawn);
//	}
//	// give the consumer the rights to trashcans
//	return std::move(trashcans);
//}



LevelDesigns::LevelDesigns(TileSpriteSheets* tile_spritesheets)
{
	m_dirt_sprite = std::make_unique<olc::Sprite>(tile_spritesheets->dirt);
	m_dirt_decal = std::make_unique<olc::Decal>(m_dirt_sprite.get());

	m_cloud_sprite = std::make_unique<olc::Sprite>(tile_spritesheets->cloud);
	m_cloud_decal = std::make_unique<olc::Decal>(m_cloud_sprite.get());

	m_exit_sprite = std::make_unique<olc::Sprite>(tile_spritesheets->exit);
	m_exit_decal = std::make_unique<olc::Decal>(m_exit_sprite.get());

	Level level1;
	level1.design += L".......................C..................C..............C.........................................";
	level1.design += L".C.........C.................C..........................................C...........C..............";
	level1.design += L".....C.............C....................C.........C............C............C......................";
	level1.design += L"............C................C.................C....................................C..............";
	level1.design += L".....C..............................C..........................L.............C.....................";
	level1.design += L".............######....######..........................##################................C.........";
	level1.design += L"......T.............................................###..................#######..................E";
	level1.design += L"##########......................#####..........#####..............................#################";
	level1.design += L"...................................................................................................";
	level1.design += L".....................................##########....................................................";
	level1.design += L"...................................................................................................";
	level1.design += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
	level1.design += L"...................................................................................................";
	level1.design += L"...................................................................................................";
	level1.design += L"...................................................................................................";
	level1.design += L"...................................................................................................";

	level1.player_init_position = { 0.0f, 1.0f };
	levels.push_back(level1);

	Level level2;
	level2.design += L".......................C..................C..............C.........................................";
	level2.design += L".C.........C.................C..........................................C...........C..............";
	level2.design += L".....C.............C....................C.........C............C............C......................";
	level2.design += L"............C................C.................C....................................C.............";
	level2.design += L".....C..............................C..........................L.............C.....................";
	level2.design += L".............######....######..........................##################................C.........";
	level2.design += L"....................................................###..................#######...................";
	level2.design += L"##########......................#####..........#####..............................#################";
	level2.design += L"...................................................................................................";
	level2.design += L".....................................##########....................................................";
	level2.design += L"...................................................................................................";
	level2.design += L"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
	level2.design += L"...................................................................................................";
	level2.design += L"...................................................................................................";
	level2.design += L"...................................................................................................";
	level2.design += L"...................................................................................................";

	level2.player_init_position = { 0.0f, 1.0f };
	levels.push_back(level2);

}

olc::vf2d LevelDesigns::get_init_player_position(int level_id)
{
	return levels[level_id].player_init_position;
}

olc::Decal* LevelDesigns::get_dirt_decal()
{
	return m_dirt_decal.get();
}

olc::Decal* LevelDesigns::get_cloud_decal()
{
	return m_cloud_decal.get();
}

olc::Decal* LevelDesigns::get_exit_decal()
{
	return m_exit_decal.get();
}

Level* LevelDesigns::get_level(int level_id)
{
	return &levels[level_id];
}

void LevelDesigns::set_static_creature(char symbol, StaticCreature* static_creature)
{
	static_creatures[symbol] = static_creature;
}


Creature::Creature() {};
Creature::Creature(olc::PixelGameEngine* engine_input)
{
	engine = engine_input;
};

olc::vi2d Creature::tile_to_px(olc::vf2d tiles, olc::vf2d camera_offset)
{
	olc::vi2d ret;
	ret.x = (tiles.x - camera_offset.x) * dims.x;
	ret.y = (tiles.y - camera_offset.y) * dims.y;
	return ret;
}

// set float position, does not update pixel position
void Creature::set_position(olc::vf2d position)
{
	pos = position;
}

// set pixel position
void Creature::set_position_px(olc::vi2d position)
{
	pos_px = position;
}

olc::vf2d Creature::get_f_tile_position()
{
	return pos;
}

olc::vi2d Creature::get_px_position(void)
{
	return pos_px;
}

olc::vi2d Creature::get_dimensions(void)
{
	return dims;
}

void Creature::set_velocity(olc::vf2d velocity)
{
	vel = velocity;
}

olc::vf2d Creature::get_velocity(void)
{
	return vel;
}


olc::vi2d Creature::emit_text_position(olc::vi2d extra_offset_px)
{
	return (pos_px + popup_text_offset + extra_offset_px);
}


StaticCreature::StaticCreature() { name = "Static Creature"; };
StaticCreature::StaticCreature(olc::PixelGameEngine* engine, const SpriteConfig* config, std::string name_input)
{
	name = name_input;
	dims = config->dims;
	scale = config->scale;
	sprite = std::make_unique<olc::Sprite>(config->image_name);
	decal = std::make_unique<olc::Decal>(sprite.get());
}

void StaticCreature::draw(float fElapsedTime)
{
	engine->DrawDecal(pos_px, decal.get(), scale);
}

Lupi::Lupi(olc::PixelGameEngine* engine, const SpriteConfig* config, std::string name_input) : StaticCreature(engine, config, name_input)
{
	name = "Lupi";
	std::vector<std::string> initial_interaction;
	initial_interaction.push_back("Hewo! Im Lupi!");
	initial_interaction.push_back("I love DAD!");
	initial_interaction.push_back("I HATE TRASHCANS!");
	initial_interaction.push_back("I have a problem......");
	initial_interaction.push_back("Would u help me? =3");
	initial_interaction.push_back("See... my dad LOVES Virginia Chestnut Trees...");
	initial_interaction.push_back("And I weally want to make him happy >:3 ");
	initial_interaction.push_back("So I have to deliver to him the world's...");
	initial_interaction.push_back("... last VIRGINIA CHESTNUT SEED!!! ");
	initial_interaction.push_back("The way to DAD is very dangerous.. :c ");
	initial_interaction.push_back("The entire path is FILLED with...");
	initial_interaction.push_back("... the evil TRASHCANS >:O ");
	initial_interaction.push_back("Take this SEED and deliver it to DAD!! ");
	initial_interaction.push_back("<LUPI GAVE YOU THE CHESTNUT SEED>");
	initial_interaction.push_back("THENK U THENK U ok i go sweep now");
	interactions.push_back(initial_interaction);
};

void Lupi::increment_interaction()
{
	m_interaction_id++;
}

void Lupi::increment_dialogue()
{
	m_dialogue_id++;
}

int Lupi::get_dialogue_id()
{
	return m_dialogue_id;
}

int Lupi::get_interaction_id()
{
	return m_interaction_id;
}

void Lupi::set_interaction_status(bool is_interacting)
{
	m_is_interacting = is_interacting;
}

bool Lupi::get_interaction_status()
{
	return m_is_interacting;
}

std::string Lupi::get_dialogue()
{
	m_is_interacting = true;
	// this means we exhausted all dialogue for that interaction
	if (m_dialogue_id >= interactions[m_interaction_id].size())
	{
		m_is_interacting = false;
		// reset the dialogue id back to first element
		m_dialogue_id = 0;
		return "";
	}
	else
	{
		std::string ret = interactions[m_interaction_id][m_dialogue_id];
		m_dialogue_id++;
		return ret;
	}

}


AnimatedCreature::AnimatedCreature() { name = "AnimatedCreature"; };
AnimatedCreature::AnimatedCreature(olc::PixelGameEngine* eng) : Creature(eng) { name = "AnimatedCreature"; };

void AnimatedCreature::update_surrounding_tiles(Level* current_level)
{
	tiles.right_tile_top = current_level->get_level_tile({ (int)(pos.x + 1.0f), (int)(pos.y) });
	tiles.right_tile_bottom = current_level->get_level_tile({ (int)(pos.x + 1.0f), (int)(pos.y + 0.9f) });
	tiles.left_tile_top = current_level->get_level_tile({ (int)(pos.x), (int)(pos.y) });
	tiles.left_tile_bottom = current_level->get_level_tile({ (int)(pos.x), (int)(pos.y + 0.9f) });
	tiles.bottom_tile_left = current_level->get_level_tile({ (int)(pos.x), (int)(pos.y + 1.0f) });
	tiles.bottom_tile_right = current_level->get_level_tile({ (int)(pos.x + 0.9f), (int)(pos.y + 1.0f) });
}

bool AnimatedCreature::is_tile_solid(Tile* tile, LevelDesigns* levels)
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

bool AnimatedCreature::check_next_to_symbol(char symbol)
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

void AnimatedCreature::resolve_collisions(LevelDesigns* levels, int level_id)
{
	// resolve collisions, making sure the tiles are not any of the static creatures
	// if they are static creatures, we should make sure we dont resolve collisions

	if ((is_tile_solid(&tiles.right_tile_top, levels) || (is_tile_solid(&tiles.right_tile_bottom, levels))))
	{
		if ((pos.x + 1.0f) > tiles.right_tile_top.n_pos.x)
		{
			pos.x = int(pos.x);
		}
	}
	if ((is_tile_solid(&tiles.left_tile_top, levels) || (is_tile_solid(&tiles.left_tile_bottom, levels))))
	{
		if ((pos.x) < tiles.left_tile_top.n_pos.x + 1.0f)
		{
			pos.x = int(pos.x + 1.0f);
		}
	}

	if ((is_tile_solid(&tiles.bottom_tile_left, levels) || (is_tile_solid(&tiles.bottom_tile_right, levels))))
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

Trashcan::Trashcan(olc::PixelGameEngine* eng, TrashCanSpriteSheets* spriteSheets, int patrol_limit) : AnimatedCreature(eng)
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
void Trashcan::set_position(olc::vf2d position, olc::vf2d camera_offset)
{
	pos = position;
	pos_px = tile_to_px(pos, camera_offset);
	prev_pos_px = pos_px;
}

void Trashcan::update_state(float fElapsedTime, olc::vf2d camera_offset)
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

void Trashcan::draw(float fElapsedTime)
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