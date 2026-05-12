#include "player.h"
#include <stdlib.h>

int set_x(int player_number) {
	// TODO
}

int set_y(int player_number) {
	// TODO
}

void *create_player(int id) {
	Player *p = malloc(sizeof(Player));
	p->id = id;
	p->x = set_x(id);
	p->y = set_y(id);
	p->health = 3;
	p->bombs = 5;
	p->radius = 3;
	return p;
}

