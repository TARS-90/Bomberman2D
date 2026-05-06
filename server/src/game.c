#include "game.h"
#include "enums.h"
#include <stdlib.h>

#define WIDTH 17
#define HEIGHT 17

struct Bomb {
	int x;
	int y;
	int time;
	int radius;
};

int *create_board() {
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



void run_game(Game *game) {
	while (!game->is_end) {
		
	}
}

void delete_game(Game *game) {
	free(game->board);
	free(game);
}


