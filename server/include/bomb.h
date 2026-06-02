#ifndef BOMB_H
#define BOMB_H

#define BOMB_IGNITION_DELAY 3000 // in miliseconds
#define BOMB_COOLDOWN	    2500

typedef struct Game Game;
typedef struct Player Player;
typedef struct Bomb {
	int x;
	int y;
	int range;
	long long placed_time;
} Bomb;

void place_bomb(Game*, Player*);
void explode(Game*, Bomb*);

#endif
