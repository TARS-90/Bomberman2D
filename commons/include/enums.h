#ifndef ENUMS_H
#define ENUMS_H

// ------ GLOBAL VARIABLES ----------
// Variables for both: client and 
// server
#define MAX_PLAYERS 4
#define FRAME_DURATION 16666
#define WIDTH	17
#define HEIGHT	17
// ----------------------------------


// -------- PLAYER COLORS -----------
// Each player has its own color
typedef enum {
	COLOR_RED,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_BROWN,
	COLOR_PURPLE,
	COLOR_DARKBLUE,
	COLOR_BLACK,
	COLOR_WHITE
} Color;
// ----------------------------------


// ---------- OBJECTS ---------------
// These are stored and displayed
// on the game board
typedef enum {
	OBJECT_EMPTY,	
	OBJECT_WALL,
	OBJECT_CHEST,
	OBJECT_BOMB,
	OBJECT_BONUS
} ObjectType;
// ----------------------------------


// ----------- PROTOCOLS ------------
// Messages types that are sent 
// between clients and server
typedef enum {
	MSG_NONE,
	MSG_MOVE_UP,
	MSG_MOVE_DOWN,
	MSG_MOVE_RIGHT,
	MSG_MOVE_LEFT,
	MSG_PLACE_BOMB,
	MSG_DISCONNECT
} MessageType;

typedef struct PlayerState {
	int x;
	int y;
	Color color;
} PlayerState;

typedef struct GameState {
	PlayerState players[MAX_PLAYERS];
	int board[WIDTH*HEIGHT];
} GameState;
// ---------------------------------

#endif
