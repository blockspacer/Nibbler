#ifndef FOOD_CELL_HPP
# define FOOD_CELL_HPP

# include "Cell.hpp"

class Board;

class FoodCell : public Cell
{
public:

	FoodCell(int x, int y, Board & board);
	~FoodCell(void);

	int				getID(void) const;

	virtual void	getHit(void);

private:

	Board &			_board;
	int				_id;

	FoodCell(void);
	FoodCell(const FoodCell & src);
	FoodCell & operator=(const FoodCell & rhs);

};

#endif
