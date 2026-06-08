#include "client.h"
#include "enums.h"
#include "sdl_engine.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <SDL3/SDL.h>


void *receive_game_state(void* args) {
	ThreadData *data = (ThreadData*) args;
	GameState buffer;

	while (1) {
		if (recv(*data->sock_fd, &buffer, sizeof(GameState), 0) <= 0) {
			printf("Connection to the server has been lost.\n");
			pthread_mutex_lock(data->mutex);
			*(data->is_connected) = 0;
			pthread_mutex_unlock(data->mutex);
			break;
		}

		pthread_mutex_lock(data->mutex);
		memcpy(data->game, &buffer, sizeof(GameState));
		pthread_mutex_unlock(data->mutex);
	}

	return NULL;
}

int connect_to_server(int *sock_fd, struct sockaddr_in *server_addr, const char *server_ip) {
	if ((*sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Creating socket failed!\n");
		return -1;
	}

	*server_addr = (struct sockaddr_in) {
		.sin_family = AF_INET,
		.sin_port = htons(12345),
		.sin_addr.s_addr = inet_addr(server_ip)
	};

	if (connect(*sock_fd, (struct sockaddr*)server_addr, sizeof(*server_addr)) < 0) {
		perror("Connecting with server failed!\n");
		return -1;
	}

	return 0;
}

void send_action(int sock_fd) {
	MessageType msg = MSG_NONE;
	// listening actions 
	switch (sdl_engine_get_action()) {
		case NONE:	 break;
		case DIR_UP:     msg = MSG_MOVE_UP; break;
		case DIR_DOWN:   msg = MSG_MOVE_DOWN; break;
		case DIR_RIGHT:  msg = MSG_MOVE_RIGHT; break;
		case DIR_LEFT:   msg = MSG_MOVE_LEFT; break;
		case PLACE_BOMB: msg = MSG_PLACE_BOMB; break;
	}
	send(sock_fd, &msg, sizeof(MessageType), MSG_NOSIGNAL);	
}

void run_client(const char *server_ip) {
	int sock_fd;
	struct sockaddr_in server_addr;
	if (connect_to_server(&sock_fd, &server_addr, server_ip) == -1) return;

	// waiting for protocol from server that starts the game
	MessageType msg;
	if (recv(sock_fd, &msg, sizeof(MessageType), 0) <= 0 || msg != MSG_START_GAME) {
		perror("Start game failed!\n");
		close(sock_fd);
		return;
	}

	// game has started
	
	GameState game_state;
	memset(&game_state, 0, sizeof(GameState));

	pthread_mutex_t game_mutex;
	pthread_mutex_init(&game_mutex, NULL);

	int is_connected = 1;

	ThreadData thread_data = (ThreadData) {
		.sock_fd = &sock_fd,
		.game = &game_state,
		.mutex = &game_mutex,
		.is_connected = &is_connected
	};

	pthread_t pid;
	if (pthread_create(&pid, NULL, receive_game_state, (void*) &thread_data) != 0) {
		perror("Creating thread for receiving game state failed!\n");
		close(sock_fd);
		pthread_mutex_destroy(&game_mutex);
		return;
	}

	if (sdl_engine_init() == -1) {
		close(sock_fd);
		pthread_mutex_destroy(&game_mutex);
		return;
	}

	SDL_Event e;
	int running = 1;
	GameState local_render_state;

	// main loop
	while (running) {
		// events listening
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				running = 0;
			}
		}

		pthread_mutex_lock(&game_mutex);
		if (!is_connected) {
			running = 0;
		}
		// coping game state to local variable to avoid locking receiving
		// game state during rendering
		memcpy(&local_render_state, &game_state, sizeof(GameState));
		pthread_mutex_unlock(&game_mutex);

		sdl_engine_render(&local_render_state);
		send_action(sock_fd);
		usleep(FRAME_DURATION);
	}

	if (game_state.is_win == 1) printf("You won!\n");
	else printf("You lost!\n");

	// releasing resources
	sdl_engine_shutdown();
	close(sock_fd);
	pthread_join(pid, NULL);
	pthread_mutex_destroy(&game_mutex);
}
