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

SDLModule::SDLModule(Nibbler & nibbler, Board & board) :
	_nibbler(nibbler), _board(board), _isGridShown(false)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
		throw NibblerException("SDL_Init() failed");

	if (!(this->_window = SDL_CreateWindow("SDL", 0, 0, board.getWidth() * CELL_WIDTH, board.getHeight() * CELL_WIDTH, SDL_WINDOW_SHOWN)))
		throw NibblerException("SDL_CreateWindow() failed");
	this->_context = SDL_GL_CreateContext(this->_window);

	if (!(this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
		throw NibblerException("SDL_CreateRenderer() failed");

	if ((IMG_Init(IMG_FLAGS) & IMG_FLAGS) != IMG_FLAGS)
		throw NibblerException("IMG_Init() failed");

	this->_initTexture(&this->_snakeHeadP1Texture[0], SNAKE_HEAD_E_IMAGE_1);
	this->_initTexture(&this->_snakeHeadP1Texture[1], SNAKE_HEAD_S_IMAGE_1);
	this->_initTexture(&this->_snakeHeadP1Texture[2], SNAKE_HEAD_W_IMAGE_1);
	this->_initTexture(&this->_snakeHeadP1Texture[3], SNAKE_HEAD_N_IMAGE_1);

	this->_initTexture(&this->_snakeBodyP1Texture[0], SNAKE_BODY_E_IMAGE_1);
	this->_initTexture(&this->_snakeBodyP1Texture[1], SNAKE_BODY_S_IMAGE_1);
	this->_initTexture(&this->_snakeBodyP1Texture[2], SNAKE_BODY_W_IMAGE_1);
	this->_initTexture(&this->_snakeBodyP1Texture[3], SNAKE_BODY_N_IMAGE_1);

	this->_initTexture(&this->_snakeHeadP2Texture[0], SNAKE_HEAD_E_IMAGE_2);
	this->_initTexture(&this->_snakeHeadP2Texture[1], SNAKE_HEAD_S_IMAGE_2);
	this->_initTexture(&this->_snakeHeadP2Texture[2], SNAKE_HEAD_W_IMAGE_2);
	this->_initTexture(&this->_snakeHeadP2Texture[3], SNAKE_HEAD_N_IMAGE_2);

	this->_initTexture(&this->_snakeBodyP2Texture[0], SNAKE_BODY_E_IMAGE_2);
	this->_initTexture(&this->_snakeBodyP2Texture[1], SNAKE_BODY_S_IMAGE_2);
	this->_initTexture(&this->_snakeBodyP2Texture[2], SNAKE_BODY_W_IMAGE_2);
	this->_initTexture(&this->_snakeBodyP2Texture[3], SNAKE_BODY_N_IMAGE_2);

	this->_initTexture(&this->_snakeDeadTexture[0], SNAKE_DEAD_E_IMAGE);
	this->_initTexture(&this->_snakeDeadTexture[1], SNAKE_DEAD_S_IMAGE);
	this->_initTexture(&this->_snakeDeadTexture[2], SNAKE_DEAD_W_IMAGE);
	this->_initTexture(&this->_snakeDeadTexture[3], SNAKE_DEAD_N_IMAGE);

	this->_initTexture(&this->_foodTexture[0], FOOD_IMAGE_0);
	this->_initTexture(&this->_foodTexture[1], FOOD_IMAGE_1);
	this->_initTexture(&this->_foodTexture[2], FOOD_IMAGE_2);
	this->_initTexture(&this->_foodTexture[3], FOOD_IMAGE_3);
	this->_initTexture(&this->_foodTexture[4], FOOD_IMAGE_4);

	this->_initTexture(&this->_enemyTexture[0], ENEMY_IMAGE_0);
	this->_initTexture(&this->_enemyTexture[1], ENEMY_IMAGE_1);
	this->_initTexture(&this->_enemyTexture[2], ENEMY_IMAGE_2);
	this->_initTexture(&this->_enemyTexture[3], ENEMY_IMAGE_3);
	this->_initTexture(&this->_enemyTexture[4], ENEMY_IMAGE_4);
	this->_initTexture(&this->_enemyTexture[5], ENEMY_IMAGE_5);
	this->_initTexture(&this->_enemyTexture[6], ENEMY_IMAGE_6);
	this->_initTexture(&this->_enemyTexture[7], ENEMY_IMAGE_7);
}

void			SDLModule::_initTexture(SDL_Texture ** target, std::string filename)
{
	*target = IMG_LoadTexture(this->_renderer, ResourceManager::getInstance().getAbsolutePathname(filename).c_str());

	if (!*target)
		throw NibblerException("IMG_LoadTexture() failed on \'" + filename + "\'");
}

SDLModule::~SDLModule(void)
{
	printf("~SDLModule()\n");
	SDL_DestroyRenderer(this->_renderer);
	SDL_DestroyWindow(this->_window);
	SDL_GL_DeleteContext(this->_context);

	for (int i = 0; i < 4; i++)
	{
		SDL_DestroyTexture(this->_snakeHeadP1Texture[i]);
		SDL_DestroyTexture(this->_snakeBodyP1Texture[i]);
		SDL_DestroyTexture(this->_snakeHeadP2Texture[i]);
		SDL_DestroyTexture(this->_snakeBodyP2Texture[i]);
		SDL_DestroyTexture(this->_snakeDeadTexture[i]);
	}
	for (int i = 0; i < FOOD_IMAGE_COUNT; i++)
		SDL_DestroyTexture(this->_foodTexture[i]);
	for (int i = 0; i < ENEMY_IMAGE_COUNT; i++)
		SDL_DestroyTexture(this->_enemyTexture[i]);
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
	this->_handleKeyHeldEvent();
}

void			SDLModule::_handleKeyHeldEvent(void)
{
	static const Uint8 *	keyState = SDL_GetKeyboardState(NULL);

	(void)keyState;

	// if (keyState[SDL_SCANCODE_1])
	// {
	// 	printf("SDLModule: pressed 1\n");
	// 	this->_nibbler.selectModule1();	
	// }
	// else if (keyState[SDL_SCANCODE_2])
	// {
	// 	printf("SDLModule: pressed 2\n");
	// 	this->_nibbler.selectModule2();
	// }
	// else if (keyState[SDL_SCANCODE_3])
	// {
	// 	printf("SDLModule: pressed 3\n");
	// 	this->_nibbler.selectModule3();
	// }
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
			// printf("SDLModule: pressed 1\n");
			this->_nibbler.selectModule1();
			return;
		case SDLK_2:
			// printf("SDLModule: pressed 2\n");
			this->_nibbler.selectModule2();
			return;
		case SDLK_3:
			// printf("SDLModule: pressed 3\n");
			this->_nibbler.selectModule3();
			return;
		case SDLK_g:
			this->_toggleGrid();
			break;


		// DEBUG

		case SDLK_TAB:
			printf("SDL Module: Pressed TAB\n");
			this->_nibbler.selectNextModule();
			return;

		case SDLK_d:
			printf("SDL Module: Pressed D\n");
			break;

		case SDLK_SPACE:
			this->_nibbler._update();
			break;
		
		// case SDLK_b:					// TEMPORARY TESTING
		// 	this->_nibbler.debugBoard();
		// 	break;
		// case SDLK_s:					// TEMPORARY TESTING
		// 	this->_nibbler.debugSnake();
		// 	break;

		default:
			break;
	}
}

void			SDLModule::_toggleGrid(void)
{
	this->_isGridShown = !this->_isGridShown;
}

void			SDLModule::render(void)
{
	SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 0);
	SDL_RenderClear(this->_renderer);
	if (this->_isGridShown)
		this->_drawGrid();
	this->_renderCells();
	SDL_RenderPresent(this->_renderer);
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

void			SDLModule::_renderCells(void)
{
	Cell *		cell;

	for (int x = 0; x < this->_board.getWidth(); x++)
		for (int y = 0; y < this->_board.getHeight(); y++)
			if ((cell = this->_board.getCell(x, y)))
				this->_renderCellAtPosition(*cell, x, y);
}

void			SDLModule::_renderCellAtPosition(Cell & cell, int x, int y)
{
	SDL_Rect	dstRect = {x * CELL_WIDTH, y * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH};
	SnakeCell *	snakeCell;
	FoodCell *	foodCell;
	EnemyCell *	enemyCell;

	if ((snakeCell = dynamic_cast<SnakeCell *>(&cell)))
		this->_renderSnakeCell(*snakeCell, dstRect);
	else if ((foodCell = dynamic_cast<FoodCell *>(&cell)))
		this->_renderFoodCell(*foodCell, dstRect);
	else if ((enemyCell = dynamic_cast<EnemyCell *>(&cell)))
		this->_renderEnemyCell(*enemyCell, dstRect);
	else
		;	// some other type of cell?
}

void			SDLModule::_renderSnakeCell(SnakeCell & snakeCell, SDL_Rect & dstRect)
{
	SDL_Texture *	texture = NULL;
	int				textureIndex = static_cast<int>(snakeCell.getDirection());

	// Player dieded
	if (snakeCell.getSnake().isDead())
		texture = this->_snakeDeadTexture[textureIndex];
	// Player 1
	else if (snakeCell.getSnake().getID() == 0)
	{
		if (snakeCell.isHead())
			texture = this->_snakeHeadP1Texture[textureIndex];
		else
			texture = this->_snakeBodyP1Texture[textureIndex];
	}
	// Player 2
	else
	{
		if (snakeCell.isHead())
			texture = this->_snakeHeadP2Texture[textureIndex];
		else
			texture = this->_snakeBodyP2Texture[textureIndex];
	}
	SDL_RenderCopy(this->_renderer, texture, NULL, &dstRect);
}

void			SDLModule::_renderFoodCell(FoodCell & foodCell, SDL_Rect & dstRect)
{
	int			textureIndex = foodCell.getID() % FOOD_IMAGE_COUNT;

	SDL_RenderCopy(this->_renderer, this->_foodTexture[textureIndex], NULL, &dstRect);	
}

void			SDLModule::_renderEnemyCell(EnemyCell & enemyCell, SDL_Rect & dstRect)
{
	int			textureIndex = enemyCell.getID() % ENEMY_IMAGE_COUNT;

	SDL_RenderCopy(this->_renderer, this->_enemyTexture[textureIndex], NULL, &dstRect);	
}


