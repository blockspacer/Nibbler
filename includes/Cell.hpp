#ifndef CELL_HPP
# define CELL_HPP

# include <string>

class Cell
{
public:
	
	Cell(int x, int y);
	virtual ~Cell(void);

	int				getX(void) const;
	int				getY(void) const;

	void			setXY(int x, int y);

	virtual void	getHit(void) = 0;

protected:

	int				_x;
	int				_y;

private:

	Cell(void);
	Cell(const Cell & src);
	Cell &	operator=(const Cell & rhs);
	
};

#endif
