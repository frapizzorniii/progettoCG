#pragma once

#include<vector>

struct Rectangle
{
	float x;
	float y;
	float width;
	float height;
};

const std::vector<Rectangle> obstacles =
{
    // x, y, width, height

    // Zona in alto a sinistra
    { 0, 0, 385, 147 },
    { 0, 147, 274, 20 },

    // Tenda viola in alto
    { 497, 0, 511, 152 },
    { 573, 152, 339, 157 },

    // Zona videogiochi
    { 1018, 0, 294, 294 },

    // Zona in alto a destra e gabbia
    { 1307, 0, 365, 86 },
    { 1398, 86, 274, 122 },
    { 1419, 208, 253, 157 },

    // Castello
    { 96, 248, 355, 31 },
    { 12, 279, 434, 70 },
    { 15, 349, 400, 51 },
    { 25, 400, 365, 66 },
    { 41, 466, 324, 40 },
    { 96, 506, 259, 26 },

    // Divano centrale
    { 502, 415, 233, 213 },

    // Tenda a destra
    { 1267, 385, 405, 187 },
    { 1236, 405, 31, 142 },

    // Zona lampada rosa
    { 1044, 425, 66, 36 },
    { 983, 461, 137, 25 },
    { 942, 486, 183, 26 },
    { 902, 512, 228, 20 },
    { 871, 532, 274, 408 },

    // Sfera in basso a destra
    { 1145, 653, 527, 287 },

    // Zona in basso a sinistra
    { 0, 658, 152, 168 },
    { 152, 689, 111, 137 },
    { 0, 826, 537, 114 },

    // Orsacchiotto
    { 542, 704, 157, 236 }
};

const int numObstacles = obstacles.size();


/*

inline bool checkCollision(Rectangle playe)
{
	for (int i = 0; i < numObstacles; i++)
	{
		if(player.x < obstacles[i].x + obstacles[i].width && 
			player.x + player.width > obstacles[i].x  &&
			player.y < obstacles[i].y + obstacles[i].height &&
			player.y + player.height > obstacles[i].y)
		{
			return true;
		}
	}
	return false;
}
*/