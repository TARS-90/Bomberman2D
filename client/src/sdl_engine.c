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
	[COLOR_BLACK]    = { .r = 20,  .g = 20,  .b = 20,  .a = 255 }, 
	[COLOR_WHITE]	 = { .r = 255, .g = 255, .b = 255, .a = 255 }
};
SDL_Window *window; // main window
SDL_Renderer *context;

int sdl_engine_init() {
	SDL_Init(SDL_INIT_VIDEO);
	
	window = SDL_CreateWindow(
		"Bomberman 2D",		// title	
		WIN_WIDTH,		// window width
		WIN_HEIGHT,		// window height
		0			// flags
	);
	if (!window) {
		perror("Create SDL_Window failed!\n");
		return -1;
	}

	context = SDL_CreateRenderer(window, NULL);
	if (!context) {
		perror("Create SDL_RenderContext failed!\n");
		return -1;
	}

	return 0;		
}

void sdl_engine_render(GameState *game) {
	SDL_Color color = COLORS[COLOR_BLACK];
	SDL_SetRenderDrawColor(context, color.r, color.g, color.b, color.a);
	SDL_RenderClear(context);

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			int is_player_drawn = 0; 

			// players
			for (int i = 0; i < MAX_PLAYERS; i++) {
				PlayerState player = game->players[i];
				if (player.x == x && player.y == y) {
					color = COLORS[player.color];
					is_player_drawn = 1;		
					break;
				}
			}

			// other objects
			if(!is_player_drawn) {
				int index = (x * HEIGHT) + y;
				ObjectType obj = game->board[index];

				switch (obj) {
					case OBJECT_EMPTY: color = COLORS[COLOR_WHITE];    break;
					case OBJECT_WALL:  color = COLORS[COLOR_BLACK];    break;
					case OBJECT_CHEST: color = COLORS[COLOR_BROWN];    break;
					case OBJECT_BONUS: color = COLORS[COLOR_PURPLE];   break;
					case OBJECT_BOMB:  color = COLORS[COLOR_DARKBLUE]; break;
				}
			}

			SDL_FRect square = {
				.x = x * SIZE,
				.y = y * SIZE,
				.w = SIZE,
				.h = SIZE
			};
			SDL_SetRenderDrawColor(context, color.r, color.g, color.b, color.a);
			SDL_RenderFillRect(context, &square);
		}
	}

	SDL_RenderPresent(context);
}

int sdl_engine_get_direction() {
	const int *key_states = SDL_GetKeyboardState(NULL);
	int vertical = 0;
	int horizontal = 0;
	int pressed_keys = 0;

	if (key_states[SDL_SCANCONDE_W]) {
		vertical += 1;
		pressed_keys++;
	}

	if (key_states[SDL_SCANCODE_S]) {
		vertical -= 1;
		pressed_keys++;
	}
	
	if (key_states[SDL_SCANCODE_D]) {
		horizontal += 1;
		pressed_keys++;
	}

	if (key_states[SDL_SCANCODE_A]) {
		horizontal -= 1;
		pressed_keys++;
	}

	if (pressed_keys == 1) {
		if (vertical == 1)    return DIR_UP;
		if (vertical == -1)   return DIR_DOWN;
		if (horizontal == 1)  return DIR_RIGHT;
		if (horizontal == -1) return DIR_LEFT;
	}
	
	return DIR_NONE;
}

void sdl_engine_shutdown() {
	if (context) {
		SDL_DestroyRenderer(context);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}

	SDL_Quit();
}

