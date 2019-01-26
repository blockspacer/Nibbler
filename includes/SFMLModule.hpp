#ifndef SFML_MODULE_HPP
# define SFML_MODULE_HPP

# include "IModule.hpp"

# include <string>
# include <SFML/Graphics.hpp>

# define CELL_WIDTH				40

class Nibbler;
class Board;
class Cell;
class SnakeCell;
class FoodCell;
class EnemyCell;

class SFMLModule : public IModule
{
public:

	SFMLModule(Board & board, std::string title);
	~SFMLModule(void);

	virtual void				disable(void);
	virtual void				enable(void);

	virtual void				handleEvents(void);
	virtual void				render(void);

private:

	Board &						_board;
	std::string					_title;

	sf::RenderWindow			_renderWindow;

	SFMLModule(void);
	SFMLModule(const SFMLModule & src);
	SFMLModule & operator=(const SFMLModule & rhs);

	void						_handleKeyPressEvent(sf::Event & event);

	void						_renderCell(Cell & cell);
	void						_renderSnakeCell(SnakeCell & snakeCell, sf::RectangleShape & rect);
	void						_renderFoodCell(FoodCell & foodCell, sf::RectangleShape & rect);
	void						_renderEnemyCell(EnemyCell & enemyCell, sf::RectangleShape & rect);


};

#endif
