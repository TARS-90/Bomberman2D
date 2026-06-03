#include "game.h"
#include "enums.h"
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

void delete_game(Game *game) {
	free(game->board);
	free(game);
}
