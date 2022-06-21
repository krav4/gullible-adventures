#pragma once
#include "olcPixelGameEngine.h"
#include "config.h"


class Creature
{
public:
	std::string name = "Creature";
	const olc::vi2d popup_text_offset = { 0, -30 };
	olc::vf2d pos = {0.0f, 0.0f};
	olc::vi2d pos_px = { 0, 0 };
	olc::vf2d vel = { 0.0f, 0.0f };
	olc::vi2d dims = {PX_TILE_SIZE_X, PX_TILE_SIZE_Y};
	olc::vf2d scale = {1.0f, 1.0f};

	olc::PixelGameEngine* engine = nullptr;
	Creature() {};
	Creature(olc::PixelGameEngine* engine_input) 
	{
		engine = engine_input;
	};
	~Creature() {};

	olc::vi2d tile_to_px(olc::vf2d tiles, olc::vf2d camera_offset)
	{
		olc::vi2d ret;
		ret.x = (tiles.x - camera_offset.x) * dims.x;
		ret.y = (tiles.y - camera_offset.y) * dims.y;
		return ret;
	}

	// set float position, does not update pixel position
	void set_position(olc::vf2d position)
	{
		pos = position;
	}

	// set pixel position
	void set_position_px(olc::vi2d position)
	{
		pos_px = position;
	}

	olc::vf2d get_f_tile_position()
	{
		return pos;
	}

	olc::vi2d get_px_position(void)
	{
		return pos_px;
	}

	olc::vi2d get_dimensions(void)
	{
		return dims;
	}

	void set_velocity(olc::vf2d velocity)
	{
		vel = velocity;
	}

	olc::vf2d get_velocity(void)
	{
		return vel;
	}


	virtual olc::vi2d emit_text_position(olc::vi2d extra_offset_px = {0, 0})
	{
		return (pos_px + popup_text_offset + extra_offset_px);
	}

	virtual void draw(float fElapsedTime) = 0;
};

typedef struct SpriteConfig
{
	std::string image_name;
	olc::vi2d dims;
	olc::vf2d scale;
} SpriteConfig;


class StaticCreature : public Creature
{
public:
	// position in global level tile coordinates
	olc::vi2d level_tile_pos;
	std::unique_ptr<olc::Decal> decal;
	std::unique_ptr<olc::Sprite> sprite;

public:
	StaticCreature() { name = "Static Creature";  };
	StaticCreature(olc::PixelGameEngine* engine, const SpriteConfig * config, std::string name_input) : Creature(engine)
	{
		name = name_input;
		dims = config->dims;
		scale = config->scale;
		sprite = std::make_unique<olc::Sprite>(config->image_name);
		decal = std::make_unique<olc::Decal>(sprite.get());
	}

	void draw(float fElapsedTime)
	{
		engine->DrawDecal(pos_px, decal.get(), scale);
	}
};

class Lupi : public StaticCreature
{
private:
	std::vector<std::vector<std::string>> interactions;
	int m_interaction_id = 0;
	int m_dialogue_id = 0;
	bool m_is_interacting = false;
public:
	Lupi(olc::PixelGameEngine* engine, const SpriteConfig* config, std::string name_input) :  StaticCreature(engine, config, name_input)
	{ 
		name = "Lupi"; 
		std::vector<std::string> initial_interaction;
		initial_interaction.push_back("Hewo! Im Lupi!");
		initial_interaction.push_back("I love DAD!");
		initial_interaction.push_back("I HATE TRASHCANS!");
		interactions.push_back(initial_interaction);
	};

	void increment_interaction()
	{
		m_interaction_id++;
	}

	void increment_dialogue()
	{
		m_dialogue_id++;
	}

	int get_dialogue_id()
	{
		return m_dialogue_id;
	}

	int get_interaction_id()
	{
		return m_interaction_id;
	}

	void set_interaction_status(bool is_interacting)
	{
		m_is_interacting = true;
	}

	std::string get_dialogue()
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

};