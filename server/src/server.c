#include "server.h"
#include "player.h"
#include "game.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Player **init_players(const int n, const int server_sock_fd) {
	Player **players = malloc(sizeof(Player*) * n);

	for (int i = 0; i < n; i++) {
		int client_sock_fd = accept(server_sock_fd, NULL, NULL);
		int id = i+1;
		printf("Player %d has connected\n", id);
		players[i] = create_player(id);
	}

	return players;
}

Game *init_game(Player **players) {
	Game *game = malloc(sizeof(Game));
	game->board = create_board();
	game->players = players;
	game->is_end = false;

	return game;
}

void run_server(const int players_count) {
	int sock_fd;
	struct sockaddr_in sock_addr;

	if (sock_fd = socket(AF_INET, SOCK_STREAM, 0) < 0) {
		printf("Error: Could not create a server socket\n");
		return;
	}

	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(12345);
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));

	listen(sock_fd, players_count);

	Player **players = init_players(players_count, sock_fd);
	Game *game = init_game(players);

	close(sock_fd);
}
