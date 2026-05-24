#ifndef SERVER_H
#define SERVER_H

typedef struct Queue Queue;
typedef struct Player Player;
typedef struct Game Game;
typedef struct GameState GameState;

Player **init_players(Queue*, const int, const int);
void run_server(const int);
void send_game_state(Game*);
void copy_players(GameState*, Game*);
void copy_game_board(GameState*, Game*);

#endif 
