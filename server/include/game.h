#ifndef GAME_H
#define GAME_H

#include "enums.h"

typedef struct Queue Queue;
typedef struct Player Player;
typedef struct Game {
	Player **players;
	Queue *bombs;
	int *board;
	int alive_players_count;
	int is_end;
	long long curr_time;
} Game;

int *create_board();
void delete_game(Game*);

#endif
