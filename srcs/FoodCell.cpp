#include "FoodCell.hpp"
#include "Board.hpp"
#include "Nibbler.hpp"

FoodCell::FoodCell(int id, int x, int y, Board & board) : Cell(x, y), _board(board)
{
	this->_id = id;
}

FoodCell::FoodCell(int x, int y, Board & board) : Cell(x, y), _board(board)
{
	this->_id = std::rand();
}

FoodCell::~FoodCell(void)
{
	
}

int		FoodCell::getID(void) const
{
	return (this->_id);
}

void	FoodCell::getHit(void)
{
	// this->_board.generateFood();
	this->_board.clearCell(this->_x, this->_y);
	Nibbler::getInstance().spawnFood();
}
