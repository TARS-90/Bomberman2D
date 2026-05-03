#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "../../enums.h"
#include <stdbool.h>
#include <stdlib.h>

#define WIDTH 17
#define HEIGHT 17

typedef struct {
	Player **players;
	int *board;
	bool is_end;
} GameState;

void* init_board();
void* init_game(Player **players);

#endif
