#include "logic.h"
#include "enums.h"
#include "queue.h"
#include "list.h"
#include "game.h"
#include "network.h"
#include "player.h"
#include "bomb.h"
#include <stdlib.h>
#include <stdio.h>


int is_tile_empty(Game *g, int x, int y) {
	int index = (y * HEIGHT) + x;
	return g->board[index] == OBJECT_EMPTY;
}

int is_tile_bonus(Game *g, int x, int y) {
	int index = (y * HEIGHT) + x;
	return g->board[index] == OBJECT_BONUS;
}

int is_tile_player(Game *g, int x, int y) {
	int is_occupied = 0;

	for (int i = 0; i < MAX_PLAYERS; i++) {
		Player *p = g->players[i];
		if (p != NULL && p->x == x && p->y == y) {
			is_occupied = 1;
		}
	}

	return is_occupied;
}

int check_move(Game *g, int x, int y) {
	// checking crossing board bounds
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return 0; // false

	// player can move if
	// there is empty tile and it isn't occupied by any player or
	// there is just bonus
	//
	// players aren't stored in board so there is need to check them
	return	( is_tile_empty(g, x, y) && !is_tile_player(g, x, y) ) || 
		is_tile_bonus(g, x, y);
}

// Creating game board as a list to store lists of tasks that 
// needs to be done on each position on the board
List *board_as_list() {
	List *board = create_list();
	if (!board) {
		perror("Malloc for results list failed!\n");
		return NULL;
	}

	for (int i = 0; i < HEIGHT*WIDTH; i++) {
		List *tasks = create_list();
		if (!tasks) {
			perror("Malloc for tasks list failed!\n");
			return NULL;
		}
		insert(board, tasks);
	}

	return board; 
}

List *process_task_queue(Queue *q, Game *g, const long long time) {
	List *results = board_as_list();	

	while (q->size) {
		Task *task = (Task*) dequeue(q);
		int player_index = task->player_id - 1;
		Player *player = g->players[player_index];
		// player position
		int x = player->x;
		int y = player->y;

		switch (task->type) {
			case MSG_DISCONNECT: {
				disconnect_player(g, task->player_id);
				free(task);
				break;
			}
			case MSG_PLACE_BOMB: {
				if (player->bombs_count > 0 && is_tile_empty(g, x, y)) {
					int index = (y * HEIGHT) + x;
					List *tasks_list = (List*) get_at(results, index);
					insert(tasks_list, task);
				}
				else {
					free(task);
				}
				
				break;
			}
			default: {
				// if task is about moving, then there is computing
				// wanted destination
				int delay = (int) time - player->last_move;
				x += ((task->type == MSG_MOVE_RIGHT) ? 1 : 0);
				x -= ((task->type == MSG_MOVE_LEFT) ? 1 : 0);
				y += ((task->type == MSG_MOVE_DOWN) ? 1 : 0);
				y -= ((task->type == MSG_MOVE_UP) ? 1 : 0);
				if (delay >= PLAYER_SPEED && check_move(g, x, y)) {
					int index = (y * HEIGHT) + x;
					List *tasks_list = (List*) get_at(results, index);
					insert(tasks_list, task);
				}
				else {
					free(task);
				}
			}
		}
	}

	return results;
}

void execute_task(Game *g, Task *t, const long long time) {
	int player_index = t->player_id - 1;
	Player *p = g->players[player_index];

	switch (t->type) {
		case MSG_MOVE_UP: {
			p->y--;
			p->last_move = time;
			break;
		}
		case MSG_MOVE_DOWN: {
			p->y++;
			p->last_move = time;
			break;
		}
		case MSG_MOVE_RIGHT: {
			p->x++;
			p->last_move = time;
			break;
		}
		case MSG_MOVE_LEFT: {
			p->x--;
			p->last_move = time;
			break;
		}
		case MSG_PLACE_BOMB: {
			place_bomb(g, p, time);
			break;
		}
	}
}

void do_tasks(Queue* q, Game *g, const long long time) {
	List *board = process_task_queue(q, g, time);
	for (int i = 0; i < WIDTH*HEIGHT; i++) {
		List *tasks = (List*) get_at(board, i);
		if (tasks->size > 0) {
			int rnd = rand() % tasks->size;
			Task *task = (Task*) get_at(tasks, rnd);
			execute_task(g, task, time);
		}
		delete_list_deep(tasks);
	}
	// using shallow deleting because the values are deleted higher in delete_list_deep()
	// to avoid double free()
	delete_list_shallow(board);
}

void process_bomb_queue(Game *g, const long long time) {
	while (g->bombs->size > 0) {
		Bomb *first_bomb = (Bomb*) g->bombs->front->value;
		if (time - first_bomb->placed_time < BOMB_IGNITION_DELAY) {
			break;
		}

		Bomb *bomb_to_explode = (Bomb*) dequeue(g->bombs);
		if (bomb_to_explode != NULL) {
			explode(g, bomb_to_explode);
		}
	}
}

int is_player_in_range(Game *g, Player *p, Bomb *b) {
	if (p->x == b->x && p->y == b->u) return 1;

	// checking UP zone
	for (int i = 1; i <= b->range; i++) {
		int check_y = b->y - i;
		if (check_y < 0) break;

		int index = (check_y * WIDTH) + b->x;
		if (g->board[index] != OBJECT_EMPTY) break;

		if (p->x == b->x && p->y == check_y) return 1;
	}

	// checking DOWN zone
	for (int i = 1; i <= b->range; i++) {
		int check_y = b->y + i;
		if (check_y < 0) break;

		int index = (check_y * WIDTH) + b->x;
		if (g->board[index] != OBJECT_EMPTY) break;

		if (p->x == b->x && p->y == check_y) return 1;
	}
	
	// checking LEFT zone
	for (int i = 1; i <= b->range; i++) {
		int check_x = b->x - i;
		if (check_x < 0) break;

		int index = (b->y * WIDTH) + check_x;
		if (g->board[index] != OBJECT_EMPTY) break;

		if (p->x == check_x && p->y == b->y) return 1;
	}

	// checking RIGHT zone
	for (int i = 1; i <= b->range; i++) {
		int check_x = b->x + i;
		if (check_x < 0) break;

		int index = (b->y * WIDTH) + check_x;
		if (g->board[index] != OBJECT_EMPTY) break;

		if (p->x == check_x && p->y == b->y) return 1;
	}

	return 0; // false
}

List *get_players_in_explosion_range(Game *g, Bomb* b) {
	List *players = create_list();
	if (!players) {
		perror("Malloc for list (players in explosion radius) failed!\n");
		return NULL;
	}

	for (int i = 0; i < MAX_PLAYERS; i++) {
		Player *p = g->players[i];
		if (!p) continue;

		if (is_player_in_range(g, p, b)) {
			insert(players, (void*) p);
		}
		
	}
}


