#ifndef ENUMS_H
#define ENUMS_H

// -------- PLAYER COLORS -----------
// Each player has its own color
typedef enum {
	COLOR_RED,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_YELLOW
} PlayerColor;
// ----------------------------------


// ---------- OBJECTS ---------------
// These are stored and displayed
// on the game board
typedef enum {
	OBJECT_EMPTY,	
	OBJECT_PLAYER,
	OBJECT_WALL,
	OBJECT_CHEST,
	OBJECT_BOMB
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
	MSG_STATE_UPDATE,
	MSG_DISCONNECT
} MessegeType;
// ---------------------------------

#endif
