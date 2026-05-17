#include "server.h"
#include "player.h"
#include "game.h"
#include "queue.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>


void *recive_tasks(void *arg) {
	Player *player = (Player*) arg;
	ThreadData data = player->tdata;
	while (1) {
		char buffer[2];
		if (recv(data.sock_fd, buffer, sizeof(buffer), 0) < 0) break; // disconected with player
		printf("Odebrano: %c\n", buffer[0]);
	}

	return NULL;
}

void connect_player(Player *player, Queue *global_queue, const int server_sock_fd) {
	int sock_fd;
	if ((sock_fd = accept(server_sock_fd, NULL, NULL)) < 0) {
		perror("Accept player connection failed!\n");
		return;
	}

	pthread_t *thread = malloc(sizeof(pthread_t*));
	player->tdata = (ThreadData) {
		.thread = thread,
		.queue = global_queue,
		.sock_fd = sock_fd,
	};
	pthread_create(thread, NULL, *recive_tasks, (void*) player);
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
		perror("`Listen failed!");
		close(sock_fd);
		return;
	}

	Queue *queue = create_queue();
	Game game = {
		.players = init_players(queue, players_count, sock_fd),
		.board = create_board(),
		.is_end = 0
	};


	close(sock_fd);
}
