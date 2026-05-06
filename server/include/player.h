#ifndef PLAYER_H
#define PLAYER_H

struct PlayerColor;

struct Player {
	int id;		// it contains sockfd
	int x;
	int y;
	int health;
	int bombs;
	int radius;
	PlayerColor color;
};

int set_x(int player_number);
int set_y(int player_number);
void* create_player(int id);

#endif
