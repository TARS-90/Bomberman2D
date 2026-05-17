#include "server.h"
#include "enums.h"
#include "player.h"
#include "game.h"
#include "queue.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

void *receive_tasks(void *arg) {
	Player *player = (Player*) arg;
	ThreadData data = player->tdata;

	while (1) {
		// TODO 
		// 1. Fixing handling player disconnection
		Message buffer;
		if (recv(data.sock_fd, &buffer, sizeof(buffer), 0) == 0) {
			printf("Lost connection with player %d\n", player->id);
			break;
		}

		Message *msg = malloc(sizeof(Message) * 2);
		msg[0] = player->id;
		msg[1] = buffer;
		enqueue(data.queue, msg);
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
	if (thread == NULL) {
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

void do_tasks(Queue* q) {
	while (q->size) {
		Message *msg = (Message*) dequeue(q);
		switch (msg[1]) {
			case MSG_MOVE_UP: {
				// TODO
				printf("Player %d wants MOVE UP\n", msg[0]);
				break;
			}
			case MSG_MOVE_DOWN: {
				// TODO
				printf("Player %d wants MOVE DOWN\n", msg[0]);
				break;
			}
			case MSG_MOVE_RIGHT: {
				// TODO
				printf("Player %d wants MOVE RIGHT\n", msg[0]);
				break;
			}
			case MSG_MOVE_LEFT: {
				// TODO
				printf("Player %d wants MOVE LEFT\n", msg[0]);
				break;
			}
			case MSG_PLACE_BOMB: {
				// TODO
				printf("Player %d wants PLACE BOMB\n", msg[0]);
				break;
			}
		}
		free(msg);
	}
}

void run_server(const int players_count) {
	int sock_fd;
	struct sockaddr_in sock_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(12345),
		.sin_addr.s_addr = INADDR_ANY
	};

	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Creation failed!");
		return;
	}

	int opt = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0) {
		perror("Bind failed!");
		close(sock_fd);
		return;
	}

	if (listen(sock_fd, players_count) < 0) {
		perror("Listen failed!");
		close(sock_fd);
		return;
	}

	// Creating resources
	Queue *queue = create_queue();
	Game game = {
		.players = init_players(queue, players_count, sock_fd),
		.board = create_board(),
		.is_end = 0
	};


	// Main game loop
	while (!game.is_end) {
		do_tasks(queue);
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
