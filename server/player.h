typedef enum {
	RED,
	GREEN,
	BLUE,
	YELLOW
} Color;

struct Player {
	int x;
	int y;
	int health;
	int bombs;
	Color color;
};
