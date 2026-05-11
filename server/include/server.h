#ifndef SERVER_H
#define SERVER_H

#include "player.h"
#include "game.h"

Player **init_players(const int, const int);
Game *init_game(Player**);
void run_server(const int);

#endif 
