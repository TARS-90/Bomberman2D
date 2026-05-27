#ifndef BOMB_H
#define BOMB_H

typedef struct Game Game;
typedef struct Player Player;
typedef struct Bomb {
	int x;
	int y;
	int range;
} Bomb;

void place_bomb(Game*, Player*);
void explode(Game*, Bomb*);

#endif
