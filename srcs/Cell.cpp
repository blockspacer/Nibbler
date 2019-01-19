#include "Cell.hpp"

Cell::Cell(int x, int y) : _x(x), _y(y) { }

Cell::~Cell(void)
{
	// printf("~Cell()\n");
}

int			Cell::getX(void) const
{
	return (this->_x);
}

int			Cell::getY(void) const
{
	return (this->_y);
}

void		Cell::setXY(int x, int y)
{
	this->_x = x;
	this->_y = y;
}

# include "SnakeCell.hpp"			// REMOVE THIS STUPID THING


std::string	Cell::toString(void) const
{
	std::string		typeStr;
	
	if (dynamic_cast<const SnakeCell *>(this))
		typeStr = "SnakeCell";
	else
		typeStr = "Cell";

	return (std::string(typeStr + "(" + std::to_string(this->_x) + ", " + std::to_string(this->_y) + ")"));
}


