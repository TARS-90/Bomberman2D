#ifndef BOMB_H
#define BOMB_H

#include <time.h>
#define BOMB_IGNITION_DELAY 3000 // in miliseconds
#define BOMB_PUT_DELAY	    1500

typedef struct Game Game;
typedef struct Player Player;
typedef struct Bomb {
	int x;
	int y;
	int range;
	time_t placed;
} Bomb;

void place_bomb(Game*, Player*);
void explode(Game*, Bomb*);

#endif
