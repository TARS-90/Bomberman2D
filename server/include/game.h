#ifndef GAME_H
#define GAME_H

typedef struct Player Player;
typedef struct Game {
	Player **players;
	int *board;
	int is_end;
} Game;

int *create_board();
void delete_game(Game*);

#endif
