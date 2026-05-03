#include "../game.h"
#include "../../enums.h"
#include <stdlib.h>

void* init_board() {
	int *board = malloc((WIDTH * HEIGHT) * sizeof(int));

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			int index = (x * HEIGHT) + y;
			if (y % 2 && x % 2) 
				board[index] = OBJECT_WALL;
			else board[index] = OBJECT_EMPTY;
		}
	}

	return board;
}

void* init_game() {
	GameState *game = malloc(sizeof(GameState));
	game->board = init_board();
	game->player_count = 2;
	game->is_end = false;

	return game;
}



