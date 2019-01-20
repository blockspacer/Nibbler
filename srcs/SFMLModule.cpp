#include "SFMLModule.hpp"
#include "Nibbler.hpp"
#include "Snake.hpp"
#include "Board.hpp"
#include "Cell.hpp"
#include "SnakeCell.hpp"
#include "FoodCell.hpp"
#include "EnemyCell.hpp"

SFMLModule::SFMLModule(Nibbler & nibbler, Board & board) :
	_nibbler(nibbler), _board(board)
{
	this->enable();
}

SFMLModule::~SFMLModule(void)
{
	printf("~SFMLModule()\n");
	this->_renderWindow.close();
}

void			SFMLModule::disable(void)
{
	// printf("SFMLModule: disable()\n");
	this->_renderWindow.close();
}

void			SFMLModule::enable(void)
{
	// printf("SFMLModule: enable()\n");
	this->_renderWindow.create(sf::VideoMode(this->_board.getWidth() * CELL_WIDTH, this->_board.getHeight() * CELL_WIDTH), "SFML");
	this->_renderWindow.setPosition(sf::Vector2i(0, 0));
	this->_renderWindow.setKeyRepeatEnabled(false);
}

void			SFMLModule::handleEvents(void)
{
	sf::Event	event;

	while (this->_renderWindow.pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
			this->_nibbler.terminate();
		else if (event.type == sf::Event::EventType::KeyPressed)
			this->_handleKeyPressEvent(event);
	}
}

void			SFMLModule::_handleKeyPressEvent(sf::Event & event)
{
	switch (event.key.code)
	{
		// Gameplay Controls
		case sf::Keyboard::Escape:
			this->_nibbler.terminate();
			break;
		case sf::Keyboard::Left:
			this->_nibbler.turnLeftP1();
			break;
		case sf::Keyboard::Right:
			this->_nibbler.turnRightP1();
			break;
		case sf::Keyboard::Numpad4:
			this->_nibbler.turnLeftP2();
			break;
		case sf::Keyboard::Numpad6:
			this->_nibbler.turnRightP2();
			break;
		case sf::Keyboard::R:
			this->_nibbler.startNewRound();
			break;
		// Graphics Controls
		case sf::Keyboard::Num1:
			// printf("SFMLModule: pressed 1\n");
			this->_nibbler.selectModule1();
			break;
		case sf::Keyboard::Num2:
			// printf("SFMLModule: pressed 2\n");
			this->_nibbler.selectModule2();
			break;
		case sf::Keyboard::Num3:
			// printf("SFMLModule: pressed 3\n");
			this->_nibbler.selectModule3();
			break;



		// DEBUG

		case sf::Keyboard::Tab:
			printf("SFML Module: Pressed TAB\n");
			this->_nibbler.selectNextModule();
			break;

		case sf::Keyboard::Space:
			this->_nibbler._update();
			break;

		case sf::Keyboard::D:
			printf("SFML Module: Pressed D\n");
			break;




		default:
			break;
	}
}


void			SFMLModule::render(void)
{
	Cell *		cell;

	this->_renderWindow.clear(sf::Color::Black);
	for (int x = 0; x < this->_board.getWidth(); x++)
		for (int y = 0; y < this->_board.getHeight(); y++)
			if ((cell = this->_board.getCell(x, y)))
				this->_renderCell(*cell);
	this->_renderWindow.display();
}

void			SFMLModule::_renderCell(Cell & cell)
{
	sf::RectangleShape	rect;

	rect.setSize(sf::Vector2f(CELL_WIDTH, CELL_WIDTH));
	rect.setPosition(cell.getX() * CELL_WIDTH, cell.getY() * CELL_WIDTH);
	try
	{
		this->_renderSnakeCell(dynamic_cast<SnakeCell &>(cell), rect);
	}
	catch (std::bad_cast& bc)
	{
		try
		{
			this->_renderFoodCell(dynamic_cast<FoodCell &>(cell), rect);
		}
		catch (std::bad_cast& bc)
		{
			this->_renderEnemyCell(dynamic_cast<EnemyCell &>(cell), rect);
		}
	}
}

void		SFMLModule::_renderSnakeCell(SnakeCell & snakeCell, sf::RectangleShape & rect)
{
	// Player dieded
	if (snakeCell.getSnake().isDead() && snakeCell.isHead())
		rect.setFillColor(sf::Color(128, 128, 128));	// Grey
	// Player 1
	else if (snakeCell.getSnake().getID() == 0)
	{
		rect.setFillColor(sf::Color::Cyan);
		if (snakeCell.isHead())
		{
			rect.setOutlineColor(sf::Color::Blue);
			rect.setOutlineThickness(20);
		}
	}
	// Player 2
	else
	{
		rect.setFillColor(sf::Color::Magenta);
		if (snakeCell.isHead())
		{
			rect.setOutlineColor(sf::Color::Red);
			rect.setOutlineThickness(20);
		}
	}
	this->_renderWindow.draw(rect);
}

void			SFMLModule::_renderFoodCell(FoodCell & foodCell, sf::RectangleShape & rect)
{
	(void)foodCell;

	rect.setFillColor(sf::Color::Yellow);
	this->_renderWindow.draw(rect);
}

void			SFMLModule::_renderEnemyCell(EnemyCell & enemyCell, sf::RectangleShape & rect)
{
	(void)enemyCell;

	rect.setFillColor(sf::Color(255, 165, 0));	// orange
	this->_renderWindow.draw(rect);
}



























