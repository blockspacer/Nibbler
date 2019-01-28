#include "SDLModule.hpp"
#include "ResourceManager.hpp"
#include "NibblerException.hpp"
#include "Nibbler.hpp"
#include "Snake.hpp"
#include "Board.hpp"
#include "Cell.hpp"
#include "SnakeCell.hpp"
#include "FoodCell.hpp"
#include "EnemyCell.hpp"

# define CELL_WIDTH			24

extern "C"
{
	IModule *	createSDLModule(Nibbler & nibbler, Board & board, std::string title)
	{
		return (new SDLModule(nibbler, board, title));
	}
}

SDLModule::SDLModule(Nibbler & nibbler, Board & board, std::string title) :
	_nibbler(nibbler),
	_board(board),
	_isGridShown(false)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
		throw NibblerException("SDL_Init() failed");

	if (!(this->_window = SDL_CreateWindow(title.c_str(), 0, 0, this->_board.getWidth() * CELL_WIDTH, this->_board.getHeight() * CELL_WIDTH, SDL_WINDOW_SHOWN)))
		throw NibblerException("SDL_CreateWindow() failed");
	this->_context = SDL_GL_CreateContext(this->_window);

	if (!(this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
		throw NibblerException("SDL_CreateRenderer() failed");

	if ((IMG_Init(IMG_FLAGS) & IMG_FLAGS) != IMG_FLAGS)
		throw NibblerException("IMG_Init() failed");

	this->_snakeHeadP1Texture = this->_initTexture(SNAKE_HEAD_P1_IMAGE);
	this->_snakeBodyP1Texture = this->_initTexture(SNAKE_BODY_P1_IMAGE);
	this->_snakeHeadP2Texture = this->_initTexture(SNAKE_HEAD_P2_IMAGE);
	this->_snakeBodyP2Texture = this->_initTexture(SNAKE_BODY_P2_IMAGE);
	this->_snakeDeadTexture = this->_initTexture(SNAKE_DEAD_IMAGE);	

	this->_foodTextures.push_back(this->_initTexture(FOOD_IMAGE_0));
	this->_foodTextures.push_back(this->_initTexture(FOOD_IMAGE_1));
	this->_foodTextures.push_back(this->_initTexture(FOOD_IMAGE_2));
	this->_foodTextures.push_back(this->_initTexture(FOOD_IMAGE_3));
	this->_foodTextures.push_back(this->_initTexture(FOOD_IMAGE_4));

	this->_enemyTextures.push_back(this->_initTexture(ENEMY_IMAGE_0));
	this->_enemyTextures.push_back(this->_initTexture(ENEMY_IMAGE_1));
	this->_enemyTextures.push_back(this->_initTexture(ENEMY_IMAGE_2));
	this->_enemyTextures.push_back(this->_initTexture(ENEMY_IMAGE_3));
	this->_enemyTextures.push_back(this->_initTexture(ENEMY_IMAGE_4));
	this->_enemyTextures.push_back(this->_initTexture(ENEMY_IMAGE_5));
	this->_enemyTextures.push_back(this->_initTexture(ENEMY_IMAGE_6));
	this->_enemyTextures.push_back(this->_initTexture(ENEMY_IMAGE_7));
}

SDL_Texture *		SDLModule::_initTexture(std::string filename)
{
	SDL_Texture *	texture = IMG_LoadTexture(this->_renderer, ResourceManager::getInstance().getBasePath(filename).c_str());

	if (!texture)
		throw NibblerException("IMG_LoadTexture() failed on \'" + filename + "\'");
	return (texture);
}

SDLModule::~SDLModule(void)
{
	printf("~SDLModule()\n");
	SDL_DestroyRenderer(this->_renderer);
	SDL_DestroyWindow(this->_window);
	SDL_GL_DeleteContext(this->_context);

	SDL_DestroyTexture(this->_snakeHeadP1Texture);
	SDL_DestroyTexture(this->_snakeBodyP1Texture);
	SDL_DestroyTexture(this->_snakeHeadP2Texture);
	SDL_DestroyTexture(this->_snakeBodyP2Texture);
	SDL_DestroyTexture(this->_snakeDeadTexture);
	for (size_t i = 0; i < this->_foodTextures.size(); i++)
		SDL_DestroyTexture(this->_foodTextures[i]);
	for (size_t i = 0; i < this->_enemyTextures.size(); i++)
		SDL_DestroyTexture(this->_enemyTextures[i]);

	IMG_Quit();
	SDL_Quit();
}

void			SDLModule::disable(void)
{
	// printf("SDLModule: disable()\n");
	SDL_HideWindow(this->_window);
}

void			SDLModule::enable(void)
{
	// printf("SDLModule: enable()\n");
	SDL_GL_MakeCurrent(this->_window, this->_context);
	SDL_ShowWindow(this->_window);
}

void			SDLModule::handleEvents(void)
{
	SDL_Event	event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			this->_nibbler.terminate();
		else if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
			this->_handleKeyPressEvent(event);
	}
}

void		SDLModule::_handleKeyPressEvent(SDL_Event & event)
{
	switch (event.key.keysym.sym)
	{
		// Gameplay Controls
		case SDLK_ESCAPE:
			this->_nibbler.terminate();
			return;
		case SDLK_LEFT:
			this->_nibbler.turnLeftP1();
			break;
		case SDLK_RIGHT:
			this->_nibbler.turnRightP1();
			break;
		case SDLK_KP_4:
			this->_nibbler.turnLeftP2();
			break;
		case SDLK_KP_6:
			this->_nibbler.turnRightP2();
			break;
		case SDLK_r:
			this->_nibbler.startNewRound();
			break;
		// Graphics Controls
		case SDLK_1:
			this->_nibbler.selectModule1();
			return;
		case SDLK_2:
			this->_nibbler.selectModule2();
			return;
		case SDLK_3:
			this->_nibbler.selectModule3();
			return;
		case SDLK_g:
			this->_toggleGrid();
			break;
		default:
			break;
	}
}

void			SDLModule::_toggleGrid(void)
{
	this->_isGridShown = !this->_isGridShown;
}

void			SDLModule::_drawGrid(void)
{
	SDL_SetRenderDrawColor(this->_renderer, 255, 255, 255, 0);
	// draw vertical lines
	int verticalLength = this->_board.getHeight() * CELL_WIDTH;
	for (int x = 0; x < this->_board.getWidth(); x++)
	{
		int xPosition = x * CELL_WIDTH;
		SDL_RenderDrawLine(this->_renderer, xPosition, 0, xPosition, verticalLength);
	}
	// draw horizontal lines
	int horizontalLength = this->_board.getWidth() * CELL_WIDTH;
	for (int y = 0; y < this->_board.getHeight(); y++)
	{
		int yPosition = y * CELL_WIDTH;
		SDL_RenderDrawLine(this->_renderer, 0, yPosition, horizontalLength, yPosition);
	}
}

void			SDLModule::render(void)
{
	Cell *		cell;

	SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 0);
	SDL_RenderClear(this->_renderer);
	if (this->_isGridShown)
		this->_drawGrid();
	for (int x = 0; x < this->_board.getWidth(); x++)
		for (int y = 0; y < this->_board.getHeight(); y++)
			if ((cell = this->_board.getCell(x, y)))
				this->_renderCell(*cell);
	SDL_RenderPresent(this->_renderer);
}

void			SDLModule::_renderCell(Cell & cell)
{
	SDL_Rect	dstRect;

	dstRect.x = cell.getX() * CELL_WIDTH;
	dstRect.y = cell.getY() * CELL_WIDTH;
	dstRect.w = CELL_WIDTH;
	dstRect.h = CELL_WIDTH;
	try
	{
		this->_renderSnakeCell(dynamic_cast<SnakeCell &>(cell), dstRect);
	}
	catch (std::bad_cast& bc)
	{
		try
		{
			this->_renderFoodCell(dynamic_cast<FoodCell &>(cell), dstRect);
		}
		catch (std::bad_cast& bc)
		{
			this->_renderEnemyCell(dynamic_cast<EnemyCell &>(cell), dstRect);
		}
	}
}

void			SDLModule::_renderSnakeCell(SnakeCell & snakeCell, SDL_Rect & dstRect)
{
	SDL_Texture *	texture;
	double			angle;

	switch (snakeCell.getDirection())
	{
		case NORTH:
			angle = 0.0;
			break;
		case EAST:
			angle = 90.0;
			break;
		case SOUTH:
			angle = 180.0;
			break;
		case WEST:
			angle = 270.0;
			break;
		default:
			angle = 0.0;
			break;
	}
	// Player dieded
	if (snakeCell.getSnake().isDead())
	{
		texture = this->_snakeDeadTexture;
	}
	// Player 1
	else if (snakeCell.getSnake().getID() == 0)
	{
		if (snakeCell.isHead())
			texture = this->_snakeHeadP1Texture;
		else
			texture = this->_snakeBodyP1Texture;
	}
	// Player 2
	else
	{
		if (snakeCell.isHead())
			texture = this->_snakeHeadP2Texture;
		else
			texture = this->_snakeBodyP2Texture;
	}
	SDL_RenderCopyEx(this->_renderer, texture, NULL, &dstRect, angle, NULL, SDL_FLIP_NONE);
}

void			SDLModule::_renderFoodCell(FoodCell & foodCell, SDL_Rect & dstRect)
{
	SDL_Texture *	texture = this->_foodTextures[foodCell.getID() % this->_foodTextures.size()];

	SDL_RenderCopy(this->_renderer, texture, NULL, &dstRect);	
}

void			SDLModule::_renderEnemyCell(EnemyCell & enemyCell, SDL_Rect & dstRect)
{
	SDL_Texture *	texture = this->_enemyTextures[enemyCell.getID() % this->_enemyTextures.size()];

	SDL_RenderCopy(this->_renderer, texture, NULL, &dstRect);	
}


