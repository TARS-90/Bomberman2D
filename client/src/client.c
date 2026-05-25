#include "client.h"
#include "enums.h"
#include "sdl_engine.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

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

	// main loop
	while (!game_state.is_end) {
		sdl_engine_render(&game_state);
		usleep(FRAME_DURATION);
	}

	// releasing resources
	sdl_engine_shutdown();
	pthread_join(pid, NULL);
	close(sock_fd);
}
