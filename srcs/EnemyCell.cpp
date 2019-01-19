#include "EnemyCell.hpp"
#include "Enemy.hpp"
#include "Board.hpp"

EnemyCell::EnemyCell(int x, int y, Enemy & enemy) : Cell(x, y), _enemy(enemy)
{
	this->_id = std::rand();
}

EnemyCell::~EnemyCell(void)
{
	// printf("~EnemyCell()\n");
}

int			EnemyCell::getID(void) const
{
	return (this->_id);
}

e_direction	EnemyCell::getDirection(void) const
{
	return (this->_enemy.getDirection());
}

void		EnemyCell::getHit(void)
{
	// Enemy stays on the board
}
