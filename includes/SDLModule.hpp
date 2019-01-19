#ifndef SDL_MODULE_HPP
# define SDL_MODULE_HPP

# include "IModule.hpp"

# include <string>
# include <SDL.h>
# include <SDL_image.h>

# define IMG_FLAGS				(IMG_INIT_JPG | IMG_INIT_PNG)

# define GHOST_IMAGE			"images/ghost.png"
# define SNAKE_IMAGE			"images/snake.png"

# define SNAKE_HEAD_E_IMAGE_1	"images/snake1/woman_shrug_E.png"
# define SNAKE_HEAD_S_IMAGE_1	"images/snake1/woman_shrug_S.png"
# define SNAKE_HEAD_W_IMAGE_1	"images/snake1/woman_shrug_W.png"
# define SNAKE_HEAD_N_IMAGE_1	"images/snake1/woman_shrug_N.png"

# define SNAKE_BODY_E_IMAGE_1	"images/snake1/dress_E.png"
# define SNAKE_BODY_S_IMAGE_1	"images/snake1/dress_S.png"
# define SNAKE_BODY_W_IMAGE_1	"images/snake1/dress_W.png"
# define SNAKE_BODY_N_IMAGE_1	"images/snake1/dress_N.png"

# define SNAKE_HEAD_E_IMAGE_2	"images/snake2/woman_ok_E.png"
# define SNAKE_HEAD_S_IMAGE_2	"images/snake2/woman_ok_S.png"
# define SNAKE_HEAD_W_IMAGE_2	"images/snake2/woman_ok_W.png"
# define SNAKE_HEAD_N_IMAGE_2	"images/snake2/woman_ok_N.png"

# define SNAKE_BODY_E_IMAGE_2	"images/snake2/kimono_E.png"
# define SNAKE_BODY_S_IMAGE_2	"images/snake2/kimono_S.png"
# define SNAKE_BODY_W_IMAGE_2	"images/snake2/kimono_W.png"
# define SNAKE_BODY_N_IMAGE_2	"images/snake2/kimono_N.png"

# define SNAKE_DEAD_E_IMAGE		"images/skull_E.png"
# define SNAKE_DEAD_S_IMAGE		"images/skull_S.png"
# define SNAKE_DEAD_W_IMAGE		"images/skull_W.png"
# define SNAKE_DEAD_N_IMAGE		"images/skull_N.png"

# define FOOD_IMAGE_COUNT		5
# define FOOD_IMAGE_0			"images/food/burger.png"
# define FOOD_IMAGE_1			"images/food/burrito.png"
# define FOOD_IMAGE_2			"images/food/hot_dog.png"
# define FOOD_IMAGE_3			"images/food/pizza.png"
# define FOOD_IMAGE_4			"images/food/taco.png"

# define ENEMY_IMAGE_COUNT		8
# define ENEMY_IMAGE_0			"images/enemy/cat.png"
# define ENEMY_IMAGE_1			"images/enemy/chick.png"
# define ENEMY_IMAGE_2			"images/enemy/chicken.png"
# define ENEMY_IMAGE_3			"images/enemy/cow.png"
# define ENEMY_IMAGE_4			"images/enemy/crab.png"
# define ENEMY_IMAGE_5			"images/enemy/dog.png"
# define ENEMY_IMAGE_6			"images/enemy/duck.png"
# define ENEMY_IMAGE_7			"images/enemy/pig.png"


class Nibbler;
class Board;
class Cell;
class SnakeCell;
class FoodCell;
class EnemyCell;

class SDLModule : public IModule
{
public:

	SDLModule(Nibbler & nibbler, Board & board);
	~SDLModule(void);

	virtual void				disable(void);
	virtual void				enable(void);

	virtual void				handleEvents(void);
	virtual void				render(void);

private:

	Nibbler &					_nibbler;
	Board &						_board;

	bool						_isGridShown;

	SDL_Window *				_window;
	SDL_Renderer *				_renderer;
	SDL_GLContext				_context;

	SDL_Texture *				_snakeHeadP1Texture[4];
	SDL_Texture *				_snakeBodyP1Texture[4];

	SDL_Texture *				_snakeHeadP2Texture[4];
	SDL_Texture *				_snakeBodyP2Texture[4];

	SDL_Texture *				_snakeDeadTexture[4];

	SDL_Texture *				_foodTexture[FOOD_IMAGE_COUNT];

	SDL_Texture *				_enemyTexture[ENEMY_IMAGE_COUNT];

	SDLModule(void);
	SDLModule(const SDLModule & src);
	SDLModule &	operator=(const SDLModule & rhs);

	void						_initTexture(SDL_Texture ** target, std::string filename);

	void						_handleKeyPressEvent(SDL_Event & event);
	void						_handleKeyHeldEvent(void);

	void						_drawGrid(void);
	void						_renderCells(void);
	void						_renderCellAtPosition(Cell & cell, int x, int y);
	void						_renderSnakeCell(SnakeCell & snakeCell, SDL_Rect & dstRect);
	void						_renderFoodCell(FoodCell & foodCell, SDL_Rect & dstRect);
	void						_renderEnemyCell(EnemyCell & enemyCell, SDL_Rect & dstRect);

	void						_toggleGrid(void);

};

#endif
