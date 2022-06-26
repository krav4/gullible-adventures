#pragma once
#include "creature.h"

constexpr float projectile_velocity = 10.0f;
constexpr float projectile_rotation_rate = 10.0f;
// offset the drawing of projectile to accound for scaling when drawing
constexpr float projectile_draw_offset_px = PX_TILE_SIZE_Y / 2;
class Projectile : public AnimatedCreature
{	
private:
	float rotation_rate = projectile_rotation_rate;
	float rotation_angle = 0.0f;
public:
	std::string spritesheet;
	olc::Decal* decal;
	bool is_enemy_hit = false;

	Projectile(olc::PixelGameEngine * eng, olc::Decal * decal_ptr) : AnimatedCreature(eng)
	{
		decal = decal_ptr;
		name = "projectile";
	}

	bool check_hit_enemy(AnimatedCreature * creature)
	{
		if (creature->is_dead)
		{
			// dont care about hitting enemy if enemy is dead already
			return false;
		}
		olc::vf2d creature_pos = creature->get_f_tile_position();
		if ((pos.x + 0.7f > creature_pos.x && pos.x < creature_pos.x + 0.7f) &&
			(pos.y + 1.0f > creature_pos.y && pos.y < creature_pos.y + 1.0f))
		{
			// signal the code that we should get rid of the projectile
			is_enemy_hit = true;
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
		rotation_angle += rotation_rate * fElapsedTime;
		olc::vi2d offset_pos_px = pos_px;
		// drawing with offset so that its in the middle of the tile when we scale it down
		offset_pos_px.y += projectile_draw_offset_px;
		engine->DrawRotatedDecal(offset_pos_px, decal, rotation_angle, { PX_TILE_SIZE_X / 2, PX_TILE_SIZE_Y / 2 }, {0.5f, 0.5f});
	}
};
