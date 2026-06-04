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


// ---------- COLORS ----------------
// Each object and player has its 
// own color
typedef enum {
	COLOR_RED,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_BROWN,
	COLOR_PURPLE,
	COLOR_DARKBLUE,
	COLOR_ORANGE,
	COLOR_BLACK,
	COLOR_WHITE
} Color;
// ----------------------------------


// ---------- ACTIONS ---------------
// Players possible actions to make 
typedef enum {
	NONE,
	DIR_UP,
	DIR_DOWN,
	DIR_RIGHT,
	DIR_LEFT,
	PLACE_BOMB
} Action;

// ---------- OBJECTS ---------------
// These are stored and displayed
// on the game board
typedef enum {
	OBJECT_EMPTY,	
	OBJECT_WALL,
	OBJECT_CHEST,
	OBJECT_BOMB,
	OBJECT_BONUS,
	OBJECT_BLAST
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
	MSG_DISCONNECT,
	MSG_START_GAME
} MessageType;

typedef struct PlayerState {
	int x;
	int y;
	Color color;
} PlayerState;

typedef struct GameState {
	PlayerState players[MAX_PLAYERS];
	int board[WIDTH*HEIGHT];
	int is_win;
} GameState;
// ---------------------------------

#endif
