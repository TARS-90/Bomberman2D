#ifndef CLIENT_H
#define CLIENT_H

#include "enums.h"
#include <pthread.h>

typedef struct sockaddr_in sockaddr_in;
typedef struct ThreadData {
	int *sock_fd;
	GameState *game;
	pthread_mutex_t *mutex;
	int *is_connected;
} ThreadData;

int connect_to_server(int*, sockaddr_in*, const char*);
void *receive_game_state(void*);
void send_action(int);
void run_client(const char*);

#endif
