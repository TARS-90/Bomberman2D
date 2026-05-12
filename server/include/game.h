#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef struct Player Player;
typedef struct Game {
	Player **players;
	int *board;
	bool is_end;
} Game;

int *create_board();
void run_game();
void delete_game(Game*);

#endif
