#pragma once
#include "olcPixelGameEngine.h"
#include "config.h"


class Creature
{
public:
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

	// set tile position, use camera offset to update pixel position
	//void set_position(olc::vf2d position, olc::vf2d camera_offset)
	//{
	//	pos = position;
	//	pos_px = tile_to_px(position, camera_offset);
	//}

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
	std::unique_ptr<olc::Decal> decal;
	std::unique_ptr<olc::Sprite> sprite;

public:
	StaticCreature() {};
	StaticCreature(olc::PixelGameEngine* engine, const SpriteConfig * config) : Creature(engine)
	{
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