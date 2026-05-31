#include "bomb.h"
#include "enums.h"
#include "queue.h"
#include "player.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void place_bomb(Game *g, Player *p, const long long time) {
	Bomb *bomb = malloc(sizeof(Bomb));
	if (!bomb) {
		perror("Malloc for bomb failed!\n");
		return;
	}

	// current player position is new bomb position
	bomb->x = p->x;
	bomb->y = p->y;
	bomb->bombs_range = p->range; // explosion range
	bomb->placed_time = time;
	p->bombs_count--;

	int index = (bomb->y * HEIGHT) + bomb->x;
	g->board[index] = OBJECT_BOMB;
	enqueue(g->bombs, bomb);
}

void explode(Game *g, Bomb *b) {
	// TODO
	//
	free(b);
}
