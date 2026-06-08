#include "bonus.h"
#include "game.h"
#include "player.h"
#include <stdlib.h>
#include <stdio.h>

Bonus *create_bonus(Game *g) {
	Bonus *b = malloc(sizeof(Bonus));
	if (!b) {
		perror("Malloc for bonus failed!\n");
		return NULL;
	}

	BonusType type = rand() % 2;
	b->type = type;
	b->placed_time = g->curr_time;
	return b;
}

void add_bonus(Player *p, Bonus *b) {
	switch (b->type) {
		case INCREASE_HEALTH: {
			if (p->health < 3) {
				p->health++;
			}
			break;
		}
		case INCREASE_BOMB_RANGE: {
			if (p->bombs_range < 6) {
				p->bombs_range++;
			}
			break;
		}
	}
	free(b);
}
