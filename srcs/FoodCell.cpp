#include "FoodCell.hpp"
#include "Board.hpp"

FoodCell::FoodCell(int x, int y, Board & board) : Cell(x, y), _board(board)
{
	this->_id = std::rand();
}

FoodCell::~FoodCell(void)
{
	// printf("~FoodCell()\n");
}

int		FoodCell::getID(void) const
{
	return (this->_id);
}

void	FoodCell::getHit(void)
{
	this->_board.generateFood();
	this->_board.clearCell(this->_x, this->_y);
}
