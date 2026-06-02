#ifndef LOGIC_H
#define LOGIC_H

typedef struct Player Player;
typedef struct Bomb Bomb;
typedef struct Game Game;
typedef struct List List;
typedef struct Queue Queue;
typedef struct Task Task;

long long get_current_time();
int is_tile_empty(Game*, int, int);
int is_tile_bonus(Game*, int, int);
int check_move(Game*, int, int);
List *board_as_list();
List *process_task_queue(Queue*, Game*);
void execute_task(Game*, Task*);
void do_tasks(Queue*, Game*);
void process_bomb_queue(Game*);
int is_player_in_range(Game*, Player*, Bomb*);
void players_in_explosion_range(Game*, Bomb*);

#endif
