#include "network.h"
#include "enums.h"
#include "queue.h"
#include "player.h"
#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

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
			task->player_id = player->id;
			task->type = MSG_DISCONNECT;
			enqueue(data.queue, task);
			printf("Lost connection with player %d\n", player->id);
			break;
		}
		
		task->player_id = player->id;
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

void disconnect_player(Game *g, int player_id) {
	int index = player_id - 1;
	Player *p = g->players[index];
	g->players[index] = NULL;
	close(p->tdata.sock_fd);
	pthread_join(*(p->tdata.thread), NULL);
	delete_player(p);
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
