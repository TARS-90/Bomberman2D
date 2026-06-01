#ifndef LOGIC_H
#define LOGIC_H

typedef struct Player Player;
typedef struct Bomb Bomb;
typedef struct Game Game;
typedef struct List List;
typedef struct Queue Queue;
typedef struct Task Task;

int is_tile_empty(Game*, int, int);
int is_tile_bonus(Game*, int, int);
int check_move(Game*, int, int);
List *board_as_list();
List *process_task_queue(Queue*, Game*, const long long);
void execute_task(Game*, Task*, const long long);
void do_tasks(Queue*, Game*, const long long);
void process_bomb_queue(Game*, const long long);
int is_player_in_range(Game*, Player*, Bomb*);
void players_in_explosion_range(Game*, Bomb*);

#endif
