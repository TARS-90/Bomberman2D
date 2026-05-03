#include "../game.h"

struct Player {
	int id;
	int x;
	int y;
	int health;
	int bombs;
	int radius;
	PlayerColor color;
};

struct Bomb {
	int x;
	int y;
	int time;
	int radius;
};

void* create_board() {
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
	Game *game = malloc(sizeof(GameState));
	game->board = create_board();
	game->is_end = false;

	return game;
}

void delete_game(Game *g) {
	free(g->board);
	free(g);
}


