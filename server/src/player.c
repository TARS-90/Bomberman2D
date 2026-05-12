#include "player.h"
#include <stdlib.h>

#define MAX_GAME_WIDTH	17
#define MAX_GAME_HEIGHT 17

// Players 1 and 3 will be at LEFT 
// Players 2 and 4 will be at RIGHT 
int set_x(int player_number) {
	if (player_number == 1 || player_number == 3) return 1;
	return MAX_GAME_WIDTH - 2;
}

// Players 1 and 4 will be at TOP 
// Players 2 and 3 will be at BOTTOM 
int set_y(int player_number) {
	if (player_number == 1 || player_number == 4) return 1;
	return MAX_GAME_HEIGHT - 2;
}

void *create_player(int id, int sock_fd) {
	Player *p = malloc(sizeof(Player));
	p->id = id;
	p->sock_fd = sock_fd;
	p->x = set_x(id);
	p->y = set_y(id);
	p->health = 3;
	p->bombs = 5;
	p->radius = 3;
	return p;
}

