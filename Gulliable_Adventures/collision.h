#pragma once

struct CollisionPadding
{
	int lower_collision_padding = 0;
	int upper_collision_padding = 0;
	int right_collision_padding = 0;
	int left_collision_padding = 0;
};

struct Collision
{
	CollisionPadding padding;
	int lower_boundary = 0;
	int upper_boundary = 0;
	int left_boundary = 0;
	int right_boundary = 0;
};
