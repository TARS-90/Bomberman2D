#ifndef PLAYER_H
#define PLAYER_H

#include "enums.h"

typedef struct Player {
	int id;		
	int sock_fd;	// it contains client socket file descriptor
	int x;
	int y;
	int health;
	int bombs;
	int radius;
} Player;

int set_x(int);
int set_y(int);
void *create_player(int, int);

#endif
