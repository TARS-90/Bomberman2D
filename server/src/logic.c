#include "logic.h"
#include "enums.h"
#include "queue.h"
#include "list.h"
#include "game.h"
#include "network.h"
#include "player.h"
#include "bomb.h"
#include "bonus.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


long long get_current_time() {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (long long) ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

int is_tile_empty(Game *g, int x, int y) {
	int index = (y * HEIGHT) + x;
	return g->board[index].type == OBJECT_EMPTY;
}

int is_tile_blast(Game *g, int x, int y) {
	int index = (y * HEIGHT) + x;
	return g->board[index].type == OBJECT_BLAST;
}

int is_tile_bonus(Game *g, int x, int y) {
	int index = (y * HEIGHT) + x;
	return g->board[index].type == OBJECT_BONUS;
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
	// there is just bonus or blast
	//
	// players aren't stored in board so there is need to check them
	return	( is_tile_empty(g, x, y) && !is_tile_player(g, x, y) ) || 
		is_tile_bonus(g, x, y) || is_tile_blast(g, x, y);
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

List *process_task_queue(Queue *q, Game *g) {
	List *results = board_as_list();	

	while (q->size) {
		Task *task = (Task*) dequeue(q);
		int player_index = task->player_id - 1;
		Player *player = g->players[player_index];
		// skiping dead player tasks
		if (!player) {
			free(task);
			continue;
		}

		// player position
		int x = player->x;
		int y = player->y;

		switch (task->type) {
			case MSG_NONE: {
				free(task);
				break;
			}
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
				int delay = (int) g->curr_time - player->last_move;
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

void execute_task(Game *g, Task *t) {
	int player_index = t->player_id - 1;
	Player *p = g->players[player_index];

	switch (t->type) {
		case MSG_MOVE_UP: {
			p->y--;
			p->last_move = g->curr_time;
			break;
		}
		case MSG_MOVE_DOWN: {
			p->y++;
			p->last_move = g->curr_time;
			break;
		}
		case MSG_MOVE_RIGHT: {
			p->x++;
			p->last_move = g->curr_time;
			break;
		}
		case MSG_MOVE_LEFT: {
			p->x--;
			p->last_move = g->curr_time;
			break;
		}
		case MSG_PLACE_BOMB: {
			place_bomb(g, p);
			break;
		}
	}
}

void do_tasks(Queue* q, Game *g) {
	List *board = process_task_queue(q, g);
	for (int i = 0; i < WIDTH*HEIGHT; i++) {
		List *tasks = (List*) get_at(board, i);
		if (tasks->size > 0) {
			int rnd = rand() % tasks->size;
			Task *task = (Task*) get_at(tasks, rnd);
			execute_task(g, task);
		}
		delete_list_deep(tasks);
	}
	// using shallow deleting because the values are deleted higher in delete_list_deep()
	// to avoid double free()
	delete_list_shallow(board);
}

void process_bomb_queue(Game *g) {
	while (g->bombs->size > 0) {
		Bomb *first_bomb = (Bomb*) g->bombs->front->value;
		if (g->curr_time - first_bomb->placed_time < BOMB_IGNITION_DELAY) {
			break;
		}

		Bomb *bomb_to_explode = (Bomb*) dequeue(g->bombs);
		if (bomb_to_explode != NULL) {
			explode(g, bomb_to_explode);
		}
	}
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
		case OBJECT_CHEST: {
			// 10% chances for creating bonus after chest destroy
			int prob = rand() % 10;
			if (prob == 0) {
				Bonus *bonus = create_bonus(g);
				g->board[index].type = OBJECT_BONUS;
				g->board[index].obj_addr = bonus;
			}
			else {
				g->board[index].type = OBJECT_EMPTY;
				g->board[index].obj_addr = NULL;
			}
			return 0;
		}
		case OBJECT_BONUS: {
			// blasts are destroying bonuses
			Bonus *obj = (Bonus*) tile.obj_addr;
			free(obj);
			create_blast_at_tile(g, index);
			return 1;
		}
		default: {
			create_blast_at_tile(g, index);
			return 1;	
		}
	}
}

void hit_players_in_explosion_range(Game *g) {
	List *players_to_kill = create_list();

	for (int i = 0; i < MAX_PLAYERS; i++) {
		Player *p = g->players[i];
		if (!p) continue;
		
		int index = (p->y * HEIGHT) + p->x;
		Tile tile = g->board[index];
		if (!p->is_untouchable && tile.type == OBJECT_BLAST) {
			p->is_untouchable = 1;
			p->last_hit = g->curr_time;
			p->health--;
			if (p->health <= 0) {
				insert(players_to_kill, p);
			}
		}
	}

	// if there is a situation when all alive players are going to die in the same time
	// the winner is randomized
	if (g->alive_players_count == players_to_kill->size) {
		int n = players_to_kill->size - 1;
		int rnd = rand() % n;
		remove_at(players_to_kill, rnd);
	}


	// kill players that have 0 or less health
	for (int i = 0; i < players_to_kill->size; i++) {
		Player *p = (Player*) get_at(players_to_kill, i);
		close(p->tdata.sock_fd);
		g->alive_players_count--;
	}

	delete_list_shallow(players_to_kill);
}

void update_all_states(Game *g) {
	// Players
	for (int i = 0; i < MAX_PLAYERS; i++) {
		Player *p = g->players[i];
		if (!p) continue;

		can_collect_bonus(g, p);

		// making players touchable if their hit delay has come
		if ((g->curr_time - p->last_hit) >= PLAYER_HIT_DELAY) {
			p->is_untouchable = 0;
		}

		// adding bombs to player equipment 
		if (p->bombs_count < 5 && (g->curr_time - p->last_bomb_add) >= BOMB_COOLDOWN) {
			p->last_bomb_add = g->curr_time;
			p->bombs_count++;	
		}
	}

	// Board 
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		switch (g->board[i].type) {
			case OBJECT_BLAST: {
				Blast *obj = (Blast*) g->board[i].obj_addr;
				if (g->curr_time - obj->placed_time >= BLAST_DELAY) {
					g->board[i].type = OBJECT_EMPTY;
					g->board[i].obj_addr = NULL;
					free(obj);
				}
				break;
			}
			case OBJECT_BONUS: {
				Bonus *obj = (Bonus*) g->board[i].obj_addr;
				if (g->curr_time - obj->placed_time >= PLACED_BONUS_DELAY) {
					g->board[i].type = OBJECT_EMPTY;
					g->board[i].obj_addr = NULL;
					free(obj);
				}
				break;
			}
		}
	}
}

void can_collect_bonus(Game* g, Player *p) {
	int index = (p->y * HEIGHT) + p->x;
	if (g->board[index].type == OBJECT_BONUS) {
		Bonus *b = (Bonus*) g->board[index].obj_addr;
		add_bonus(p, b);
		g->board[index].type = OBJECT_EMPTY;
		g->board[index].obj_addr = NULL;
	}
}

int is_game_over(Game *g) {
	if (g->alive_players_count <= 1) return 1;
	return 0;
}

Player *find_winner(Game *g) {
	Player *winner = NULL;

	for (int i = 0; i < MAX_PLAYERS; i++) {
		Player *p = g->players[i];
		if (p != NULL) {
			winner = p;
		}
	}

	return winner;
}
