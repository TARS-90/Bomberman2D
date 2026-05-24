#ifndef LOGIC_H
#define LOGIC_H

typedef struct Game Game;
typedef struct List List;
typedef struct Queue Queue;
typedef struct Task Task;

int is_tile_empty(Game*, int, int);
int is_tile_bonus(Game*, int, int);
int check_move(Game*, int, int);
List *board_as_list();
List *process_task_queue(Queue*, Game*);
void execute_task(Game*, Task*);
void do_tasks(Queue*, Game*);

#endif
