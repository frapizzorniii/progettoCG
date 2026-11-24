#pragma once

enum PlayerState
{
	WALK_UP,
	WALK_DOWN,
	WALK_LEFT,
	WALK_RIGHT,
	IDLE
};

struct Player {
	float x;
	float y;
	float width;
	float height;
	float collisionWidth;
	float collisionHeight;
	float speed;
	PlayerState state;
	PlayerState lastDirection;
	int frame;						// 1 = dx, 0 = sx
};

Player player =
{
	775, 700, 50, 75, 25, 20, 150, IDLE, WALK_UP
};

const char* selectImage() {
	const char* image;

	if (player.state == WALK_DOWN) {
		if (player.frame == 0)
			image = "player/down_1.png";
		else 
			image = "player/down_2.png";
	}

	else if (player.state == WALK_UP) {
		if (player.frame == 0)
			image = "player/up_1.png";
		else
			image = "player/up_2.png";
	}

	else if (player.state == WALK_RIGHT) {
		if (player.frame == 0)
			image = "player/right_1.png";
		else
			image = "player/right_2.png";
	}

	else if (player.state == WALK_LEFT) {
		if (player.frame == 0)
			image = "player/left_1.png";
		else
			image = "player/left_2.png";
	}

	else 
	{
		if (player.lastDirection == WALK_DOWN)
			image = "player/down_idle.png";
		else if (player.lastDirection == WALK_UP)
			image = "player/up_idle.png";
		else if (player.lastDirection == WALK_LEFT)
			image = "player/left_idle.png";
		else 
			image = "player/right_idle.png";
	}

	return image;
}

