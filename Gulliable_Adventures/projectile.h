#pragma once
#include "creature.h"

constexpr float projectile_velocity = 10.0f;
class Projectile : public AnimatedCreature
{
public:
	std::string spritesheet;
	olc::Decal* decal;

	Projectile(olc::PixelGameEngine * eng, olc::Decal * decal_ptr) : AnimatedCreature(eng)
	{
		decal = decal_ptr;
		name = "projectile";
	}

	bool check_hit_enemy(AnimatedCreature * creature)
	{
		olc::vf2d creature_pos = creature->get_f_tile_position();
		if ((pos.x + 0.7f > creature_pos.x && pos.x < creature_pos.x + 0.7f) &&
			(pos.y + 0.7f > creature_pos.y))
		{

			return true;
		}
		else
		{
			return false;
		}
	}

	void update_state(float fElapsedTime, olc::vf2d camera_offset)
	{
		pos += vel * fElapsedTime;
		pos_px = tile_to_px(pos, camera_offset);
	}

	virtual void draw(float fElapsedTime) override
	{
		engine->DrawDecal(pos_px, decal);
	}
};