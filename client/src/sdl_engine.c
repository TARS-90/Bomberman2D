#include "sdl_engine.h"
#include "enums.h"
#include <stdio.h>

const SDL_Color COLORS[] = {
	[COLOR_RED]      = { .r = 220, .g = 20,  .b = 60,  .a = 255 }, 
	[COLOR_BLUE]     = { .r = 30,  .g = 144, .b = 255, .a = 255 },
	[COLOR_GREEN]    = { .r = 34,  .g = 139, .b = 34,  .a = 255 }, 
	[COLOR_YELLOW]   = { .r = 255, .g = 215, .b = 0,   .a = 255 }, 
	[COLOR_BROWN]    = { .r = 139, .g = 69,  .b = 19,  .a = 255 }, 
	[COLOR_PURPLE]   = { .r = 128, .g = 0,   .b = 128, .a = 255 }, 
	[COLOR_DARKBLUE] = { .r = 0,   .g = 0,   .b = 139, .a = 255 }, 
	[COLOR_BLACK]    = { .r = 20,  .g = 20,  .b = 20,  .a = 255 }  
};
SDL_Window *window; // main window
SDL_RenderContex *context;

void sdl_engine_init() {
	SDL_Init();

	window = SDL_CreateWindow(
		"Bomberman 2D",		// title	
		WIN_WIDTH,		// window width
		WIN_HEIGHT,		// window height
		0			// flags
	);
	if (!window) {
		perror("Create SDL_Window failed!\n");
		return;
	}

	if((surface = SDL_GetWindowSurface(window)) == NULL) {
		perror("Getting SDL_Surface failed!\n");
		sdl_engine_shutdown();
	}
}

void sdl_engine_render(GameState *game) {
	SDL_Color c = COLORS[COLOR_BLACK];
	SDL_SetRenderDrawColor(context, c.r, c.g, c.b, c.a);
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {

			// drawing players
			PlayerState player;
			int is_player_draw = 0; 
			for (int i = 0; i < MAX_PLAYERS; i++) {
				player = game->players[i];
				if (player.x == x && player.y == y) {

					is_player_draw = 1;		
					break;
				}
			}

			// drawing other objects
			if(!is_player_draw) {

			}
			
		}
	}
}

void sdl_engine_shutdown() {
	if (window) {
		SDL_DestroyWindow();
	}
}

