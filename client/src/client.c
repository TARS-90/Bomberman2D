#include "client.h"
#include "enums.h"
#include "sdl_engine.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <SDL3/SDL.h>

typedef struct ThreadData {
	int *sock_fd;
	GameState *game;
} ThreadData;

void *receive_game_state(void* args) {
	ThreadData *data = (ThreadData*) args;

	while (1) {
		if (recv(*data->sock_fd, data->game, sizeof(GameState), 0) <= 0) {
		    printf("Połączenie z serwerem przerwane.\n");
		    break;
		}		
	}
}

int connect_to_server(int *sock_fd, struct sockaddr_in *server_addr) {
	if ((*sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Creating socket failed!\n");
		return -1;
	}

	*server_addr = (struct sockaddr_in) {
		.sin_family = AF_INET,
		.sin_port = htons(12345),
		.sin_addr.s_addr = inet_addr("127.0.0.1")
	};

	if (connect(*sock_fd, (struct sockaddr*)server_addr, sizeof(*server_addr)) < 0) {
		perror("Connecting with server failed!\n");
		return -1;
	}

	return 0;
}

void send_action(int sock_fd) {
	MessageType msg = MSG_NONE;
	switch (sdl_engine_get_direction()) {
		case DIR_NONE:  break;
		case DIR_UP:    msg = MSG_MOVE_UP; break;
		case DIR_DOWN:  msg = MSG_MOVE_DOWN; break;
		case DIR_RIGHT: msg = MSG_MOVE_RIGHT; break;
		case DIR_LEFT:  msg = MSG_MOVE_LEFT; break;
	}
	if (msg != MSG_NONE) {
		send(sock_fd, &msg, sizeof(MessageType), 0);	
	}
}

void run_client() {
	int sock_fd;
	struct sockaddr_in server_addr;
	if (connect_to_server(&sock_fd, &server_addr) == -1) return;

	// waiting for protocol from server that starts the game
	MessageType msg;
	if (recv(sock_fd, &msg, sizeof(MessageType), 0) <= 0 || msg != MSG_START_GAME) {
		perror("Start game failed!\n");
		close(sock_fd);
		return;
	}

	// game has started

	GameState game_state;
	game_state.is_end = 0;

	ThreadData thread_data = (ThreadData) {
		.sock_fd = &sock_fd,
		.game = &game_state
	};
	pthread_t pid;
	if (pthread_create(&pid, NULL, receive_game_state, (void*) &thread_data) != 0) {
		perror("Creating thread for receiving game state failed!\n");
		close(sock_fd);
		return;
	}

	if (sdl_engine_init() == -1) {
		close(sock_fd);
		return;
	}

	SDL_Event e;
	int running = 1;

	// main loop
	while (running && !game_state.is_end) {
		printf("NOWA KLATKA!\n");
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				printf("Kliknięto X!\n");
				running = 0;
			}
		}
		sdl_engine_render(&game_state);
		send_action(sock_fd);
		usleep(FRAME_DURATION);
	}

	// releasing resources
	sdl_engine_shutdown();
	close(sock_fd);
	pthread_join(pid, NULL);
}
