#ifndef SERVER_H
#define SERVER_H

typedef struct Task Task;
typedef struct Queue Queue;
typedef struct Player Player;
typedef struct Game Game;

void connect_player(Player*, Queue*, const int);
Player **init_players(Queue*, const int, const int);
void *receive_tasks(void*);
void do_tasks(Queue*);
void run_server(const int);

#endif 
