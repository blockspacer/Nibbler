#include "SnakeCell.hpp"
#include "Snake.hpp"

SnakeCell::SnakeCell(int x, int y, Snake & snake, e_direction direction, bool isHead, size_t index) :
	Cell(x, y), _snake(snake), _direction(direction), _isHead(isHead), _index(index)
{
	
}

SnakeCell::~SnakeCell(void)
{
	// printf("~SnakeCell()\n");
}

Snake &		SnakeCell::getSnake(void) const
{
	return (this->_snake);
}

e_direction	SnakeCell::getDirection(void) const
{
	return (this->_direction);
}

bool		SnakeCell::isHead(void) const
{
	return (this->_isHead);
}

void		SnakeCell::setDirection(e_direction direction)
{
	this->_direction = direction;
}

void		SnakeCell::getHit(void)
{
	if (this->_isHead)
		this->_snake.die();
	else
		this->_snake.getHitAtIndex(this->_index);
}
