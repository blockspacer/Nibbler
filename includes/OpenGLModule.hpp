#ifndef OPEN_GL_MODULE_HPP
# define OPEN_GL_MODULE_HPP

# define SCREEN_WIDTH		800
# define SCREEN_HEIGHT		800

# include "IModule.hpp"
# include "Shader.hpp"

# include <string>
# include <GL/glew.h>
# include <GLFW/glfw3.h>

class Shader;
class Nibbler;
class Board;
class Cell;
class SnakeCell;
class FoodCell;
class EnemyCell;



class OpenGLModule : public IModule
{
public:

	OpenGLModule(Nibbler & nibbler, Board & board);
	~OpenGLModule(void);

	virtual void			disable(void);
	virtual void			enable(void);

	virtual void			handleEvents(void);
	virtual void			render(void);

private:

	Nibbler &				_nibbler;
	Board &					_board;

	GLFWwindow *			_window;
	Shader *				_shader;

	OpenGLModule(void);
	OpenGLModule(const OpenGLModule & src);
	OpenGLModule & operator=(const OpenGLModule & rhs);

	void					_initOpenGLStuff(void);
	void					_initVertexObjects(void);

	void					_renderCells(void);
	void					_renderCellAtPosition(Cell & cell, int x, int y);
	// void					_renderSnakeCell(SnakeCell & snakeCell, sf::RectangleShape & rect);
	// void					_renderFoodCell(FoodCell & foodCell, sf::RectangleShape & rect);
	// void					_renderEnemyCell(EnemyCell & enemyCell, sf::RectangleShape & rect);

	

};

#endif
