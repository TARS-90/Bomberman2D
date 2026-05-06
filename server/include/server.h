#ifndef SERVER_H
#define SERVER_H

struct Player;

void **init_players(const int n);
void *init_game(Player **players);
void run_server();
#endif 
