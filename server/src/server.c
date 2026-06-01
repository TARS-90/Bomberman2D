#include "server.h"
#include "enums.h"
#include "network.h"
#include "logic.h"
#include "player.h"
#include "game.h"
#include "queue.h"
#include "list.h"
#include "bomb.h"
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

Player **init_players(Queue *global_queue, const int n, const int server_sock_fd) {
	Player **players = malloc(sizeof(Player*) * MAX_PLAYERS);

	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (i < n) {
			int id = i+1;
			Player *player = create_player(id);
			connect_player(player, global_queue, server_sock_fd);
			players[i] = player;
		}
		else {
			players[i] = NULL;
		}
	}

	return players;
}

void run_server(const int players_count) {
	// Creating resources
	int sock_fd;
	if (init_socket(&sock_fd, players_count) < 0) return;

	struct timespec ts;
	long long last_add_bomb_time = 0;
	Queue *queue = create_queue();
	Game game = {
		.players = init_players(queue, players_count, sock_fd),
		.bombs = create_queue(),
		.board = create_board(),
		.is_end = 0
	};

	
	// sending protocol to each player to start game
	send_start_game(&game);

	// Main game loop
	while (!game.is_end) {
		clock_gettime(CLOCK_MONOTONIC, &ts);
		// getting time in miliseconds
		long long curr_time = (long long) ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

		send_game_state(&game);
		do_tasks(queue, &game, curr_time);
		process_bomb_queue(&game, curr_time);
		add_bombs_to_players(&game, &last_add_bomb_time, curr_time);
		usleep(FRAME_DURATION);
	}


	// Release resources
	close(sock_fd);
	for (int i = 0; i < MAX_PLAYERS; i++) {
		delete_player(game.players[i]);
	}
	free(game.players);
	free(game.board);
	delete_queue_deep(game.bombs);
	delete_queue_deep(queue);
}

void copy_players(GameState *game_state, Game *g) {
	for (int i = 0; i < MAX_PLAYERS; i++) {
		Player *p = g->players[i];
		if (p != NULL) {
			game_state->players[i] = (PlayerState) {
				.x = p->x,
				.y = p->y,
				.color = p->id - 1
			};
		} 
		else {
			game_state->players[i] = (PlayerState) { -1, -1, -1 }; // it means that there is no player
		}
	}
}

void copy_game_board(GameState *game_state, Game *g) {
	for (int i = 0; i < HEIGHT*WIDTH; i++) {
		game_state->board[i] = g->board[i];
	}
}

void send_game_state(Game *g) { 
	GameState game_state;
	copy_players(&game_state, g);
	copy_game_board(&game_state, g);
	game_state.is_end = g->is_end;

	for (int i = 0; i < MAX_PLAYERS; i++) {
		Player *p = g->players[i];
		if (p != NULL) {
			send(p->tdata.sock_fd, &game_state, sizeof(GameState), MSG_NOSIGNAL);
		}
	}
}

void send_start_game(Game *g) {
	MessageType msg = MSG_START_GAME;
	for (int i = 0; i < MAX_PLAYERS; i++) {
		Player *p = g->players[i];
		if (p != NULL) {
			send(p->tdata.sock_fd, &msg, sizeof(MessageType), MSG_NOSIGNAL);
		}
	}
}

void add_bombs_to_players(Game *g, long long *last_add_time, const long long curr_time) {
	if (curr_time - *last_add_time >= BOMB_COOLDOWN)	{
		for (int i = 0; i < MAX_PLAYERS; i++) {
			Player *p = g->players[i];
			if (p != NULL && p->bombs_count < 5) {
				p->bombs_count++;	
			}
		}
		*last_add_time = curr_time;
	}
}
