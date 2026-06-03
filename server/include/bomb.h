#ifndef BOMB_H
#define BOMB_H

#define BOMB_IGNITION_DELAY 3000 // in miliseconds
#define BOMB_COOLDOWN	    2500
#define BLAST_DELAY 1000

typedef struct Game Game;
typedef struct Player Player;
typedef struct Blast {
	long long placed_time;
} Blast;
typedef struct Bomb {
	int x;
	int y;
	int range;
	long long placed_time;
} Bomb;

void place_bomb(Game*, Player*);
void explode(Game*, Bomb*);
void make_blast(Game*, Bomb*);
void create_blast_at_tile(Game*, int);

#endif
