#include "game.h"
#include "enums.h"
#include <stdlib.h>

int *create_board() {
	int *board = malloc((WIDTH * HEIGHT) * sizeof(int));

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			int index = (y * HEIGHT) + x;
			if (y % 2 && x % 2) 
				board[index] = OBJECT_WALL;
			else board[index] = OBJECT_EMPTY;
		}
	}

	return board;
}

void delete_game(Game *game) {
	free(game->board);
	free(game);
}
