#include "bomb.h"
#include "enums.h"
#include "queue.h"
#include "player.h"
#include "game.h"
#include <stdio.h>


void place_bomb(Game *g, Player *p) {
	Bomb *bomb = malloc(sizeof(Bomb));
	if (!bomb) {
		perror("Malloc for bomb failed!\n");
		return;
	}

	// current player position is new bomb position
	bomb->x = p->x;
	bomb->y = p->y;
	bomb->range = p->range; // explosion range
	p->bombs--;

	int index = (bomb->y * WIDTH) + bomb->x;
	g->board[index] = OBJECT_BOMB;
	enqueue(g->bombs, bomb);
}

void expolode(Game *g, Bomb *b) {
	// TODO
	//
	free(b);
}
