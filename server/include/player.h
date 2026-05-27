#ifndef PLAYER_H
#define PLAYER_H

#include <pthread.h>

typedef struct Queue Queue;
typedef struct ThreadData {
	pthread_t *thread;
	Queue *queue;		// shared queue
	int sock_fd;
} ThreadData;

typedef struct Player {
	int id;		
	ThreadData tdata;
	int x;
	int y;
	int health;
	int bombs;
	int range;
} Player;

int set_x(int);
int set_y(int);
Player *create_player(int);
void delete_player(Player*);

#endif
