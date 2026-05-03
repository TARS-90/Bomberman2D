#ifndef GAME_H
#define GAME_H

#include "../../enums.h"
#include <stdbool.h>
#include <stdlib.h>

#define WIDTH 17
#define HEIGHT 17
#define MAX_PLAYERS 4
#define MAX_BOMBS 32

typedef struct Player;
typedef struct Bomb;
typedef struct {
	Player *players[MAX_PLAYERS];
	int *board;
	bool is_end;
} Game;

void create_player(int id);
void* create_board();
void* init_game();
void delete_game(Game *g);

#endif
