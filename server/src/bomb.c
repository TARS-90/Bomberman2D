#include "bomb.h"
#include "enums.h"
#include "queue.h"
#include "player.h"
#include "game.h"
#include "logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void place_bomb(Game *g, Player *p) {
	Bomb *bomb = malloc(sizeof(Bomb));
	if (!bomb) {
		perror("Malloc for bomb failed!\n");
		return;
	}

	// current player position is new bomb position
	bomb->x = p->x;
	bomb->y = p->y;
	bomb->range = p->bombs_range; // explosion range
	bomb->placed_time = g->curr_time;
	p->bombs_count--;

	int index = (bomb->y * HEIGHT) + bomb->x;
	g->board[index].type = OBJECT_BOMB;
	g->board[index].obj_addr = (void*) bomb;
	enqueue(g->bombs, bomb);
}

void explode(Game *g, Bomb *b) {
	// Deleting bomb from board
	int index = (b->y * HEIGHT) + b->x;
	g->board[index].type = OBJECT_EMPTY;
	g->board[index].obj_addr = NULL;

	make_blast(g, b);
	free(b);
}

int process_blast_tile(Game *g, int x, int y) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return 0;

	int index = (y * HEIGHT) + x;
	Tile tile = g->board[index];

	switch (tile.type) { 
		case OBJECT_WALL: return 0; // indestructible
		case OBJECT_BOMB: return 0; // objects
		case OBJECT_BLAST: {
			Blast *blast = (Blast*) tile.obj_addr;
			blast->placed_time = g->curr_time;
			return 1;
		}
		default: {
			Blast *blast = malloc(sizeof(Blast));
			if (!blast) {
				perror("Malloc for blast failed!\n");
				return 0;
			}
			
			blast->placed_time = g->curr_time;
			g->board[index].type = OBJECT_BLAST;
			g->board[index].obj_addr = blast;
			return 1;	
		}
	}
}

void make_blast(Game *g, Bomb *b) {
	// Directions: UP, DOWN, RIGHT, LEFT
	int dx[] = {0, 0, 1, -1};
	int dy[] = {-1, 1, 0, 0};

	// Same position as exploded bomb
	process_blast_tile(g, b->x, b->y);
	for (int dir = 0; dir < 4; dir++) {
		for (int i = 1; i < b->range; i++) {
			int check_x = b->x + (dx[dir] * i);
			int check_y = b->y + (dy[dir] * i);

			// if blast hit something indestructible, then stop
			// spreading at this direction
			if (!process_blast_tile(g, check_x, check_y)) {
				break;
			}
		}
	}
}

