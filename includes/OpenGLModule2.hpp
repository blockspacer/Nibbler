#ifndef OPEN_GL_MODULE_2_HPP
# define OPEN_GL_MODULE_2_HPP

# define SCREEN_WIDTH		800
# define SCREEN_HEIGHT		800
# define ASPECT_RATIO		((float)SCREEN_WIDTH / SCREEN_HEIGHT)

# define SCALE_INC			1.02f
# define SCALE_DEC			0.98f
# define ROTATE_INC			1.0f
# define TRANSLATE_INC		0.05f

# include "IModule.hpp"
# include "Shader.hpp"

# include <string>
# include <GL/glew.h>
# include <SFML/Window.hpp>
# include <SFML/OpenGL.hpp>

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>

# define GLM_ENABLE_EXPERIMENTAL
# include <glm/gtx/string_cast.hpp>

class Shader;
class Nibbler;
class Board;
class Cell;
class SnakeCell;
class FoodCell;
class EnemyCell;
class Model;

class OpenGLModule2 : public IModule
{
public:

	OpenGLModule2(Nibbler & nibbler, Board & board);
	~OpenGLModule2(void);

	virtual void			disable(void);
	virtual void			enable(void);

	virtual void			handleEvents(void);
	virtual void			render(void);

private:

	Nibbler &				_nibbler;
	Board &					_board;

	sf::Window				_window;
	Shader *				_shader;

	Model *					_testModel;
	Model *					_catModel;

	glm::mat4				_transformMatrix;
	glm::mat4				_viewMatrix;
	glm::mat4				_projectionMatrix;

	glm::vec3				_lightPosition;

	bool					_isWireframe;
	bool					_isDefaultUV;
	bool					_isShaded;

	float					_scale_x;
	float					_scale_y;
	float					_scale_z;

	float					_degrees_rotate_x;
	float					_degrees_rotate_y;
	float					_degrees_rotate_z;

	float					_translate_x;
	float					_translate_y;
	float					_translate_z;

	OpenGLModule2(void);
	OpenGLModule2(const OpenGLModule2 & src);
	OpenGLModule2 & operator=(const OpenGLModule2 & rhs);

	void					_initOpenGLStuff(void);
	void					_initVertexObjects(void);

	void					_handleKeyPressEvent(sf::Event & event);

	void					_renderCells(void);
	void					_renderCellAtPosition(Cell & cell, int x, int y);
	// void					_renderSnakeCell(SnakeCell & snakeCell, sf::RectangleShape & rect);
	// void					_renderFoodCell(FoodCell & foodCell, sf::RectangleShape & rect);
	// void					_renderEnemyCell(EnemyCell & enemyCell, sf::RectangleShape & rect);

	void					_toggleWireframeMode(void);
	void					_toggleDefaultUV(void);
	void					_toggleShading(void);

	void					_updateMatrix(void);

	void					_resetGraphicsParameters(void);


	void					_drawModelAtPosition(Model & model, int x, int y);
	

};

#endif
