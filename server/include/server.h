#ifndef SERVER_H
#define SERVER_H

typedef struct Queue Queue;
typedef struct Player Player;
typedef struct Game Game;

Player **init_players(Queue*, const int, const int);
Game *init_game(Player**);
void *recive_tasks(void*);
void run_server(const int);

#endif 
