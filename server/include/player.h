#ifndef PLAYER_H
#define PLAYER_H

#include "enums.h"

typedef struct {
	int id;		// it contains sockfd
	int x;
	int y;
	int health;
	int bombs;
	int radius;
	PlayerColor color;
} Player;

int set_x(int);
int set_y(int);
void *create_player(int, int);

#endif
