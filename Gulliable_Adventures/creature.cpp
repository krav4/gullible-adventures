
#include "olcPixelGameEngine.h"
#include "config.h"
#include "creature.h"
#include "level.h"
#include <string.h>
#include <unordered_map>

Creature::Creature() {};
Creature::Creature(olc::PixelGameEngine* engine_input)
{
	engine = engine_input;
};

Creature::Creature(const Creature& original)
{
	pos = original.pos;
	pos_px = original.pos_px;
	dims = original.dims;
	engine = original.engine;
	name = original.name;
	scale = original.scale;
	vel = original.vel;
}

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
StaticCreature::StaticCreature(olc::PixelGameEngine* engine, const SpriteConfig* config, std::string name_input) : Creature(engine)
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

AnimatedCreature::AnimatedCreature() 
{
	name = "AnimatedCreature";
};
AnimatedCreature::AnimatedCreature(olc::PixelGameEngine* eng) : Creature(eng) 
{ 
	name = "AnimatedCreature"; 
};
AnimatedCreature::AnimatedCreature(const AnimatedCreature& original) : Creature(original)
{

}



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
		tile->symbol != LEVEL_DESIGN_TRASHCAN &&
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
	m_spritesheets = *spriteSheets;
	health_points = 3;
	animation_interval = 0.08f;
	px_patrol_limit = patrol_limit;
	create_decals();
}

Trashcan::Trashcan(const Trashcan& original) : AnimatedCreature(original)
{
	health_points = original.health_points;
	animation_interval = original.animation_interval;
	m_spritesheets = original.m_spritesheets;
	px_patrol_limit = original.px_patrol_limit;
	create_decals();
}

void Trashcan::create_decals()
{
	walk_right_sprite = std::make_unique<olc::Sprite>(m_spritesheets.walk_right_spritesheet);
	walk_right_decal = std::make_unique<olc::Decal>(walk_right_sprite.get());
	walk_right_animation = std::make_unique<SpriteAnimation>();
	walk_right_animation->SetParams(animation_interval, walk_right_sprite->width, walk_right_sprite->height, m_spritesheets.walk_tile_cols, m_spritesheets.walk_tile_rows, m_spritesheets.walk_tile_count);

	walk_left_sprite = std::make_unique<olc::Sprite>(m_spritesheets.walk_left_spritesheet);
	walk_left_decal = std::make_unique<olc::Decal>(walk_left_sprite.get());
	walk_left_animation = std::make_unique<SpriteAnimation>();
	walk_left_animation->SetParams(animation_interval, walk_left_sprite->width, walk_left_sprite->height, m_spritesheets.walk_tile_cols, m_spritesheets.walk_tile_rows, m_spritesheets.walk_tile_count);

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

	// kinematics
	pos += vel * fElapsedTime;
	pos_px = tile_to_px(pos, camera_offset);

	// if we reached the patrolling limit, now flip the trashcan
	if (tiles.bottom_tile_right.symbol == LEVEL_DESIGN_EMPTY && is_pointing_right)
	{
		is_pointing_right = false;
	}
	else if (tiles.bottom_tile_left.symbol == LEVEL_DESIGN_EMPTY && !is_pointing_right)
	{
		is_pointing_right = true;
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