#include "EnemyCell.hpp"
#include "Enemy.hpp"
#include "Board.hpp"

EnemyCell::EnemyCell(int x, int y, Enemy & enemy) :
	Cell(x, y),
	_enemy(enemy)
{
	
}

EnemyCell::~EnemyCell(void)
{
	
}

int			EnemyCell::getID(void) const
{
	return (this->_enemy.getID());
}

e_direction	EnemyCell::getDirection(void) const
{
	return (this->_enemy.getDirection());
}

void		EnemyCell::getHit(void)
{
	// Enemy stays on the board
}
