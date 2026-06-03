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

	Queue *queue = create_queue();
	Game game = {
		.players = init_players(queue, players_count, sock_fd),
		.bombs = create_queue(),
		.board = create_board(),
		.alive_players_count = players_count,
		.curr_time = 0
	};
	
	// sending protocol to each player to start game
	send_start_game(&game);

	// Main game loop
	while (!is_game_over(&game)) {
		game.curr_time = get_current_time();

		send_game_state(&game);
		do_tasks(queue, &game);
		process_bomb_queue(&game);
		hit_players_in_explosion_range(&game);
		update_all_states(&game);

		usleep(FRAME_DURATION);
	}

	Player *winner = find_winner(&game);
	if (winner != NULL) {
		printf("Player %d has win!\n", winner->id);
		disconnect_player(&game, winner->id);
	}

	// Release resources
	close(sock_fd);
	delete_game(&game);
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
		game_state->board[i] = g->board[i].type;
	}
}

void send_game_state(Game *g) { 
	GameState game_state;
	copy_players(&game_state, g);
	copy_game_board(&game_state, g);
	game_state.is_end = 0;

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
