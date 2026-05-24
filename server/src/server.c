#include "server.h"
#include "enums.h"
#include "network.h"
#include "logic.h"
#include "player.h"
#include "game.h"
#include "queue.h"
#include "list.h"
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

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
	// Creating resources
	int sock_fd;
	if (init_socket(&sock_fd, players_count) < 0) return;

	Queue *queue = create_queue();
	Game game = {
		.players = init_players(queue, players_count, sock_fd),
		.board = create_board(),
		.is_end = 0
	};


	send_game_state(&game, players_count);
	// Main game loop
	while (!game.is_end) {
		if (queue->size > 0) {
			do_tasks(queue, &game);
			send_game_state(&game, players_count);
		}
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



void copy_players(GameState *game_state, Game *g, int player_count) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (i < player_count && g->players[i] != NULL) {
            Player *p = g->players[i];
            game_state->players[i] = (PlayerState) {
                .x = p->x,
                .y = p->y,
                .color = p->id - 1
            };
        } else {
            game_state->players[i] = (PlayerState) { -1, -1, -1 };
        }
    }
}

void copy_game_board(GameState *game_state, Game *g) {
	for (int i = 0; i < HEIGHT*WIDTH; i++) {
		game_state->board[i] = g->board[i];
	}
}

void send_game_state(Game *g, const int player_count) { 
	GameState game_state;
	copy_players(&game_state, g, player_count);
	copy_game_board(&game_state, g);

	for (int i = 0; i < player_count; i++) {
		Player *p = g->players[i];
		if (p != NULL) {
			send(p->tdata.sock_fd, &game_state, sizeof(GameState), 0);
		}
	}
}
