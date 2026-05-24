#include "server.h"
#include "enums.h"
#include "player.h"
#include "game.h"
#include "queue.h"
#include "list.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

struct Task {
	int id;	// player id
	MessageType type;
}; 

void *receive_tasks(void *arg) {
	Player *player = (Player*) arg;
	ThreadData data = player->tdata;

	while (1) {
		Task *task = malloc(sizeof(Task));
		if (!task) {
			perror("Malloc for task failed!\n");
			break;
		}

		MessageType buffer;
		if (recv(data.sock_fd, &buffer, sizeof(buffer), 0) <= 0) {
			task->id = player->id;
			task->type = MSG_DISCONNECT;
			enqueue(data.queue, task);
			printf("Lost connection with player %d\n", player->id);
			break;
		}
		
		task->id = player->id;
		task->type = buffer;
		enqueue(data.queue, task);
	}

	return NULL;
}

void connect_player(Player *player, Queue *global_queue, const int server_sock_fd) {
	int sock_fd;
	if ((sock_fd = accept(server_sock_fd, NULL, NULL)) < 0) {
		perror("Accept player connection failed!\n");
		free(player);
		return;
	}

	pthread_t *thread = malloc(sizeof(pthread_t));
	if (!thread) {
		perror("Malloc for thread failed!\n");
		close(sock_fd);
		free(player);
		return;
	}

	player->tdata = (ThreadData) {
		.thread = thread,
		.queue = global_queue,
		.sock_fd = sock_fd,
	};

	if (pthread_create(thread, NULL, receive_tasks, (void*) player) != 0) {
		perror("Create thread failed!\n");
		close(sock_fd);
		free(thread);
		free(player);
		return;
	}
	
	printf("Player %d has connected\n", player->id);
}


Player **init_players(Queue *global_queue, const int n, const int server_sock_fd) {
	Player **players = malloc(sizeof(Player*) * n);

	for (int i = 0; i < n; i++) {
		int id = i+1;
		Player *player = create_player(id);
		connect_player(player, global_queue, server_sock_fd);
		players[i] = player;
	}

	return players;
}

int init_socket(int *sock_fd, const int players_count) {
	struct sockaddr_in sock_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(12345),
		.sin_addr.s_addr = INADDR_ANY
	};

	if ((*sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Creation failed!");
		return -1;
	}

	int opt = 1;
	setsockopt(*sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(*sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0) {
		perror("Bind failed!");
		close(*sock_fd);
		return -1;
	}

	if (listen(*sock_fd, players_count) < 0) {
		perror("Listen failed!");
		close(*sock_fd);
		return -1;
	}

	return 0;
}

void run_server(const int players_count) {
	// Creating resources
	int sock_fd;
	if (init_socket(&sock_fd, players_count) < 0) return;

	Queue *queue = create_queue();
	Game game = {
		.players = init_players(queue, players_count, sock_fd),
		.board = create_board(),
		.is_end = 0
	};


	// Main game loop
	while (!game.is_end) {
		do_tasks(queue, &game);
		sleep(1);
	}


	// Release resources
	close(sock_fd);
	for (int i = 0; i < players_count; i++) {
		Player *p = game.players[i];
		if (p != NULL) {
			pthread_join(*p->tdata.thread, NULL);
			free(p->tdata.thread);
			free(p);
		}
	}
	free(game.players);
	free(game.board);
	delete_queue_deep(queue);
	free(queue);
}




#define HEIGHT 17
#define WIDTH  17
int is_tile_empty(Game *g, int x, int y) {
	int index = (x * HEIGHT) + y;
	return g->board[index] == OBJECT_EMPTY;
}

int is_tile_bonus(Game *g, int x, int y) {
	int index = (x * HEIGHT) + y;
	return g->board[index] == OBJECT_BONUS;
}

int check_move(Game *g, int x, int y) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return 0; // false

	return is_tile_empty(g, x, y) || is_tile_bonus(g, x, y);
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
		int player_index = task->id - 1;
		// player position
		int x = g->players[player_index]->x;
		int y = g->players[player_index]->y;

		switch (task->type) {
			case MSG_DISCONNECT: {
				// TODO
				free(task);
				break;
			}
			case MSG_PLACE_BOMB: {
				if (is_tile_empty(g, x, y)) {
					int index = (x * HEIGHT) + y;
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
				x += ((task->type == MSG_MOVE_RIGHT) ? 1 : 0);
				x -= ((task->type == MSG_MOVE_LEFT) ? 1 : 0);
				y += ((task->type == MSG_MOVE_DOWN) ? 1 : 0);
				y -= ((task->type == MSG_MOVE_UP) ? 1 : 0);
				if (check_move(g, x, y)) {
					int index = (x * HEIGHT) + y;
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
	int player_index = t->id - 1;
	Player *p = g->players[player_index];

	switch (t->type) {
		case MSG_MOVE_UP: {
			p->y--;
			break;
		}
		case MSG_MOVE_DOWN: {
			p->y++;
			break;
		}
		case MSG_MOVE_RIGHT: {
			p->x++;
			break;
		}
		case MSG_MOVE_LEFT: {
			p->x--;
			break;
		}
		case MSG_PLACE_BOMB: {
			// TODO
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
