#include "game.h"
#include "enums.h"
#include "queue.h"
#include "player.h"
#include "bomb.h"
#include "network.h"
#include <stdlib.h>

Tile *create_board() {
	Tile *board = malloc((WIDTH * HEIGHT) * sizeof(Tile));

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			int index = (y * HEIGHT) + x;
			if (y % 2 && x % 2){
				board[index].type = OBJECT_WALL;
				board[index].obj_addr = NULL;
			}
			else {
				board[index].type = OBJECT_EMPTY;
				board[index].obj_addr = NULL;
			}
		}
	}

	return board;
}

void delete_game(Game *g) {
	for (int i = 0; i < WIDTH*HEIGHT; i++) {
		Tile tile = g->board[i];
		switch (tile.type) {
			case OBJECT_BOMB: {
				Bomb *obj = (Bomb*) tile.obj_addr;
				free(obj);
				break;
			}
			case OBJECT_BLAST: {
				Blast *obj = (Blast*) tile.obj_addr;
				free(obj);
				break;
			}
			case OBJECT_BONUS: {
				// TODO
			}
		}
	}

	// values in nodes are released higher
	delete_queue_shallow(g->bombs);
	free(g->board);
	free(g->players);
}
