#ifndef SERVER_H
#define SERVER_H

typedef struct Player Player;
typedef struct Game Game;

Player **init_players(const int, const int);
Game *init_game(Player**);
void run_server(const int);

#endif 
