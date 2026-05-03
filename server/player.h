#ifndef PLAYER_H
#define PLAYER_H

#include "../enums.h"

typedef struct {
	int x;
	int y;
	int health;
	int bombs;
	int radius;
	PlayerColor color;
} Player;

#endif
