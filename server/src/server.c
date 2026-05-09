#include "server.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Player **init_players(const int n, const int server_fd) {
	Player **players = malloc(sizeof(Player*) * n);

	for (int i = 0; i < n; i++) {
		int id = accept(server_fd, NULL, NULL);
		int player_number = i+1;
		printf("Player %d has connected\n", player_number);
		Player *p = create_player(id, player_number);
		players[i] = p;
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
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = INADDR_ANY;

	bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
	listen(server_fd, players_count);
	printf("Waiting for the players...");

	Player **players = init_players(players_count, server_fd);
	Game *game = init_game(players);

	close(server_fd);
}
