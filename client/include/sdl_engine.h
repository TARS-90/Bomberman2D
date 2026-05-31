#ifndef SDL_ENGINE_H
#define SDL_ENGINE_H

#include <SDL3/SDL.h>

#define SIZE 20	// size of one block 
#define WIN_WIDTH  900
#define WIN_HEIGHT 600

typedef struct GameState GameState;

int  sdl_engine_init(void);
void sdl_engine_render(GameState*);
void sdl_engine_poll_event();
int  sdl_engine_get_action();
void sdl_engine_shutdown();

#endif
