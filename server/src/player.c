#include "player.h"
#include <stdlib.h>

int set_x(int player_number) {
	// TODO
}

int set_y(int player_number) {
	// TODO
}

void *create_player(int id, int player_number) {
	Player *p = malloc(sizeof(Player));
	p->id = id;
	p->x = set_x(player_number);
	p->y = set_y(player_number);
	p->health = 3;
	p->bombs = 5;
	p->radius = 3;
	p->color = player_number;
	return p;
}

