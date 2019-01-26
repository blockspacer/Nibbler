#include "Board.hpp"
#include "SnakeCell.hpp"
#include "FoodCell.hpp"
#include <iostream>

Board::Board(int width, int height) : _width(width), _height(height)
{
	this->_cells.resize(this->_width * this->_height);

	// TESTING
	// std::shared_ptr<Cell> tempCell = std::make_shared<Cell>(0, 1);
	// this->setCell(tempCell);		// NO LEAKS :)

}

Board::~Board(void) { }

int		Board::getWidth(void) const
{
	return (this->_width);
}

int		Board::getHeight(void) const
{
	return (this->_height);
}

Cell *	Board::getCell(int x, int y) const
{
	return (this->_cells[this->_width * y + x].get());
}

bool	Board::isEmptyCell(int x, int y) const
{
	return (this->_cells[this->_width * y + x].get() == nullptr);
}

bool	Board::isValidPosition(int x, int y) const
{
	return (0 <= x && x < this->_width && 0 <= y && y < this->_height);
}

void	Board::setCell(const std::shared_ptr<Cell> & cell)
{
	int		x = cell->getX();
	int		y = cell->getY();

	this->_cells[this->_width * y + x] = cell;
}

void	Board::setCell(const std::shared_ptr<Cell> & cell, int x, int y)
{
	cell->setXY(x, y);
	this->_cells[this->_width * y + x] = cell;
}

void	Board::clearCell(int x, int y)
{
	this->_cells[this->_width * y + x].reset();
}

void	Board::clearAllCells(void)
{
	for (int i = 0; i < this->_width * this->_height; i++)
		this->_cells[i].reset();
}

void	Board::debug(void)
{
	Cell *	cell;

	std::cout << "Board::debug()" << std::endl;
	for (int i = 0; i < this->_width * this->_height; i++)
		if ((cell = this->_cells[i].get()))
			std::cout << cell->toString() << std::endl;
	std::cout << std::endl;
}

bool	Board::_findEmptyPosition(int & emptyX, int & emptyY)
{
	int		startX = std::rand() % this->_width;
	int		startY = std::rand() % this->_height;

	int		x = startX;
	int		y = startY;

	while (true)
	{
		if (this->getCell(x, y) == nullptr)
		{
			emptyX = x;
			emptyY = y;
			return (true);
		}
		x += 1;
		if (x == this->_width)
		{
			x = 0;
			y = (y + 1) % this->_height;
		}
		if (x == startX && y == startY)
			return (false);
	}
}

void	Board::generateFood(void)
{
	int		emptyX;
	int		emptyY;

	if (!this->_findEmptyPosition(emptyX, emptyY))
		return;

	this->setCell(std::make_shared<FoodCell>(emptyX, emptyY, *this));
}







