#ifndef MESSAGE_HPP
# define MESSAGE_HPP

enum	e_message
{
	BOARD_INFO = 42,
	SNAKE_SPAWNED,
	
	P1_TURN_LEFT,
	P1_TURN_RIGHT,
	P2_TURN_LEFT,
	P2_TURN_RIGHT,


	START_NEW_ROUND,
	NEW_FOOD_SPAWNED,
	NEW_ENEMY_SPAWNED,
	
};

#endif
