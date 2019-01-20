#ifndef BOARD_HPP
# define BOARD_HPP

# include "Cell.hpp"
# include <vector>

class Board
{
public:

	Board(int width, int height);
	~Board(void);

	int			getWidth(void) const;
	int			getHeight(void) const;
	Cell *		getCell(int x, int y) const;

	bool		isEmptyCell(int x, int y) const;
	bool		isValidPosition(int x, int y) const;

	void		setCell(const std::shared_ptr<Cell> & cell);
	void		setCell(const std::shared_ptr<Cell> & cell, int x, int y);
	void		clearCell(int x, int y);
	void		clearAllCells(void);

	void		debug(void);

	void		generateFood(void);

private:

	int									_width;
	int									_height;

	std::vector<std::shared_ptr<Cell>>	_cells;

	Board(void);
	Board(const Board & src);
	Board &	operator=(const Board & rhs);

	bool		_findEmptyPosition(int & emptyX, int & emptyY);

};

#endif
