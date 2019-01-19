#include "Enemy.hpp"
#include "EnemyCell.hpp"
#include "Board.hpp"
#include "Cell.hpp"

Enemy::Enemy(int x, int y, e_direction direction, Board & board) : _isDead(false), _direction(direction), _board(board)
{
	this->_enemyCell = std::make_shared<EnemyCell>(x, y, *this);
	this->_board.setCell(this->_enemyCell);
}

Enemy::~Enemy(void)
{
	// printf("~Enemy()\n");
}

bool		Enemy::isDead(void) const
{
	return (this->_isDead);
}

e_direction	Enemy::getDirection(void) const
{
	return (this->_direction);
}

void		Enemy::_getNextXY(int & nextX, int & nextY) const
{
	int		currentX = this->_enemyCell->getX();
	int		currentY = this->_enemyCell->getY();

	switch(this->_direction)
	{
		case EAST:
			nextX = currentX + 1;
			nextY = currentY;
			break;
		case SOUTH:
			nextX = currentX;
			nextY = currentY + 1;
			break;
		case WEST:
			nextX = currentX - 1;
			nextY = currentY;
			break;
		case NORTH:
			nextX = currentX;
			nextY = currentY - 1;
			break;
	}
}

void		Enemy::update(void)
{
	if (this->_isDead)
		return;

	int		currentX = this->_enemyCell->getX();
	int		currentY = this->_enemyCell->getY();
	int		nextX;
	int		nextY;
	Cell *	nextCell;

	this->_getNextXY(nextX, nextY);
	this->_board.clearCell(currentX, currentY);
	if (this->_board.isValidPosition(nextX, nextY))
	{
		if ((nextCell = this->_board.getCell(nextX, nextY)))
			nextCell->getHit();
		this->_board.setCell(this->_enemyCell, nextX, nextY);
	}
	else
		this->_isDead = true;
}
