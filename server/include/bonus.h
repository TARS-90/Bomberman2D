#ifndef BONUS_H
#define BONUS_H

#define PLACED_BONUS_DELAY 5000 // in miliseconds

typedef enum {
	INCREASE_HEALTH,
	INCREASE_BOMB_RANGE
} BonusType;

typedef struct Player Player;
typedef struct Game Game;
typedef struct Bonus {
	BonusType type;
	long long placed_time;
} Bonus;

Bonus *create_bonus(Game*);
void add_bonus(Player*, Bonus*);

#endif
