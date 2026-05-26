#ifndef	NETWORK_H
#define NETWORK_H

#include "enums.h"

typedef struct Player Player;
typedef struct Queue Queue;
typedef struct Game Game;
typedef struct Task {
	int player_id;	
	MessageType type;
} Task; 

void *recive_tasks(void*);
void connect_player(Player*, Queue*, const int);
void disconnect_player(Game*, int);
int  init_socket(int*, const int);

#endif
