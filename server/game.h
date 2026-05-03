#ifndef GAME_H
#define GAME_H

#define WIDTH 17
#define HEIGHT 17

#include <stdbool.h>

typedef struct {
	int *board;
	int player_count;
	bool is_end;
} GameState;

void* init_game();

#endif
