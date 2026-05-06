#ifndef GAME_H
#define GAME_H

#define MAX_PLAYERS 4

typedef struct {
	Player *players[MAX_PLAYERS];
	int *board;
	bool is_end;
} Game;

void* create_board();
void run_game();
void delete_game(Game *g);

#endif
