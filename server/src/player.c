#include "player.h"
#include "enums.h"
#include "game.h"
#include <stdlib.h>

/*
 *	HORIZONTAL POSITIONING
 *
 *  Players 1 and 3 will be at LEFT
 *  Players 2 and 4 will be at RIGHT
*/
int set_x(int player_number) {
	if (player_number == 1 || player_number == 3) return 0;
	return WIDTH - 1;
}

/*
 *	VERTICAL POSITIONING
 *
 *  Players 1 and 4 will be at TOP
 *  Players 2 and 3 will be at BOTTOM
*/
int set_y(int player_number) {
	if (player_number == 1 || player_number == 4) return 0;
	return HEIGHT - 1;
}

Player *create_player(int id) {
	Player *p = malloc(sizeof(Player));
	p->id = id;
	p->tdata = (ThreadData) {	// before connection
		.thread = NULL,
		.queue = NULL,
		.sock_fd = 0
	};	
	p->x = set_x(id);
	p->y = set_y(id);
	p->health = 3;
	p->bombs = 5;
	p->radius = 3;
	return p;
}

void delete_player(Player *p) {
	if (p != NULL) {
		free(p->tdata.thread);
		free(p);
	}
}

