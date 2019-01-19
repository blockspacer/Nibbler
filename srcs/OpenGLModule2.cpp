#include "OpenGLModule2.hpp"
#include "ResourceManager.hpp"
#include "NibblerException.hpp"
#include "Nibbler.hpp"
#include "Snake.hpp"
#include "Board.hpp"
#include "Cell.hpp"
#include "SnakeCell.hpp"
#include "FoodCell.hpp"
#include "EnemyCell.hpp"

#include "Model.hpp"


#include <iostream>

# define WALL_MODEL			"models/cube.obj"


// # define TEST_MODEL			"models/spider.obj"
# define TEST_MODEL			"models/cube.obj"
// # define TEST_MODEL			"models/42.obj"
// # define TEST_MODEL			"models/teapot.obj"
// # define TEST_MODEL			"models/teapot2.obj"





// float vertices2[] =
// {
//     -0.5f, -0.5f, 0.0f,
//      0.5f, -0.5f, 0.0f,
//      0.0f,  0.5f, 0.0f
// };



OpenGLModule2::OpenGLModule2(Nibbler & nibbler, Board & board) :
	_nibbler(nibbler), _board(board)
{
	this->_initOpenGLStuff();
	// this->_initVertexObjects();
	this->_shader = new Shader();

	this->_testModel = new Model(TEST_MODEL);


	this->_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	this->_projectionMatrix = glm::perspective(glm::radians(45.0f), ASPECT_RATIO, 0.1f, 100.0f);
	
	
	this->_resetGraphicsParameters();


	


	

}

void			OpenGLModule2::_initOpenGLStuff(void)
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 4;
	settings.minorVersion = 0;
	settings.attributeFlags = sf::ContextSettings::Core;

	this->_window.create(
		sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
		"OpenGL 2",
		sf::Style::Default,
		settings);

	/*
		"Error. Unable to create the context. Retrying without shared context.
		Warning. New context created without shared context."

		WTF DOES THAT MEAN?
	*/

	GLenum		err = glewInit();
	if (err != GLEW_OK)
		throw NibblerException("glewInit() failed:\n" +
			std::string(reinterpret_cast<const char *>(glewGetErrorString(err))));

	this->_window.setPosition(sf::Vector2i(0, 0));
	this->_window.setKeyRepeatEnabled(false);

	glEnable(GL_DEPTH_TEST);
}

# define STRIDE_LEN 3

// REMOVE THIS PROBABLY
void			OpenGLModule2::_initVertexObjects(void)
{
	// unsigned int	vbo;
	// unsigned int	vao;

	// glGenVertexArrays(1, &vao);
	// glGenBuffers(1, &vbo);
	// glBindVertexArray(vao);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	// // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * STRIDE_LEN * 3 * m->obj.triangle_count, m->gl_data, GL_STATIC_DRAW);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE_LEN * sizeof(float), (void *)(0 * sizeof(float)));
	// glEnableVertexAttribArray(0);
}

OpenGLModule2::~OpenGLModule2(void)
{
	printf("~OpenGLModule2()\n");
	this->_window.close();
	delete this->_shader;

	delete this->_testModel;
}

void			OpenGLModule2::disable(void)
{
	this->_window.setActive(false);
	this->_window.setVisible(false);
	
}

void			OpenGLModule2::enable(void)
{
	this->_window.setActive(true);
	this->_window.setVisible(true);
}

void			OpenGLModule2::handleEvents(void)
{
	sf::Event	event;

	while (this->_window.pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
			this->_nibbler.terminate();
		else if (event.type == sf::Event::EventType::KeyPressed)
			this->_handleKeyPressEvent(event);
	}



	if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp))
	{
		this->_scale_x *= SCALE_INC;
		this->_scale_y *= SCALE_INC;
		this->_scale_z *= SCALE_INC;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown))
	{
		this->_scale_x *= SCALE_DEC;
		this->_scale_y *= SCALE_DEC;
		this->_scale_z *= SCALE_DEC;
	}

	// scale
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7))
			this->_scale_x *= SCALE_DEC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))
			this->_scale_x *= SCALE_INC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
			this->_scale_y *= SCALE_DEC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
			this->_scale_y *= SCALE_INC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
			this->_scale_z *= SCALE_DEC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
			this->_scale_z *= SCALE_INC;
	}
	// translate
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7))
			this->_translate_x -= TRANSLATE_INC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))
			this->_translate_x += TRANSLATE_INC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
			this->_translate_y -= TRANSLATE_INC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
			this->_translate_y += TRANSLATE_INC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
			this->_translate_z -= TRANSLATE_INC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
			this->_translate_z += TRANSLATE_INC;
	}
	// rotate
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7))
			this->_degrees_rotate_x -= ROTATE_INC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))
			this->_degrees_rotate_x += ROTATE_INC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
			this->_degrees_rotate_y -= ROTATE_INC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
			this->_degrees_rotate_y += ROTATE_INC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
			this->_degrees_rotate_z -= ROTATE_INC;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
			this->_degrees_rotate_z += ROTATE_INC;
	}

	
}

void			OpenGLModule2::_resetGraphicsParameters(void)
{
	this->_isWireframe = true;
	glPolygonMode(GL_FRONT_AND_BACK, this->_isWireframe ? GL_LINE : GL_FILL);

	this->_isDefaultUV = true;
	this->_shader->setBool("is_default_uv", this->_isDefaultUV);

	this->_isShaded = false;
	this->_shader->setBool("is_shaded", this->_isShaded);

	this->_lightPosition = glm::vec3(0.0f, 5.0f, 5.0f);
	this->_shader->setLightPosition(
		const_cast<float *>(glm::value_ptr(this->_lightPosition)));

	this->_scale_x = 1.0f;
	this->_scale_y = 1.0f;
	this->_scale_z = 1.0f;
	this->_degrees_rotate_x = 0.0f;
	this->_degrees_rotate_y = 0.0f;
	this->_degrees_rotate_z = 0.0f;
	this->_translate_x = 0.0f;
	this->_translate_y = 0.0f;
	this->_translate_z = 0.0f;
}

void			OpenGLModule2::_handleKeyPressEvent(sf::Event & event)
{
	switch (event.key.code)
	{
		// Gameplay controls
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
		// Graphics controls
		case sf::Keyboard::Num1:
			this->_nibbler.selectModule1();
			break;
		case sf::Keyboard::Num2:
			this->_nibbler.selectModule2();
			break;
		case sf::Keyboard::Num3:
			this->_nibbler.selectModule3();
			break;
		case sf::Keyboard::Numpad0:
			this->_resetGraphicsParameters();
			break;
		case sf::Keyboard::W:
			this->_toggleWireframeMode();
			break;
		case sf::Keyboard::U:
			this->_toggleDefaultUV();
			break;
		case sf::Keyboard::S:
			this->_toggleShading();
			break;
		






		// DEBUG

		case sf::Keyboard::Tab:
			printf("OpenGL 2 Module: Pressed TAB\n");
			this->_nibbler.selectNextModule();
			break;

		case sf::Keyboard::Space:
			this->_nibbler._update();
			break;

		case sf::Keyboard::D:
			printf("OpenGL 2 Module: Pressed D\n");
			break;




		default:
			break;
	}
}

void			OpenGLModule2::_toggleWireframeMode(void)
{
	this->_isWireframe = !this->_isWireframe;
	glPolygonMode(GL_FRONT_AND_BACK, this->_isWireframe ? GL_LINE : GL_FILL);
}

void			OpenGLModule2::_toggleDefaultUV(void)
{
	this->_isDefaultUV = !this->_isDefaultUV;
	this->_shader->setBool("is_default_uv", this->_isDefaultUV);
}

void			OpenGLModule2::_toggleShading(void)
{
	this->_isShaded = !this->_isShaded;
	this->_shader->setBool("is_shaded", this->_isShaded);
}

void			OpenGLModule2::_updateMatrix(void)
{
	glm::mat4		model_matrix = glm::mat4(1.0f);

	model_matrix = glm::translate(model_matrix, glm::vec3(this->_translate_x, this->_translate_y, this->_translate_z));
	model_matrix = glm::rotate(model_matrix, glm::radians(this->_degrees_rotate_x), glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::radians(this->_degrees_rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::radians(this->_degrees_rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));
	model_matrix = glm::scale(model_matrix, glm::vec3(this->_scale_x, this->_scale_y, this->_scale_z));
		
	this->_shader->setTransformMatrix(
		const_cast<float *>(glm::value_ptr(this->_projectionMatrix * this->_viewMatrix * model_matrix)));

	// this->_shader->setTransformMatrix(
	// 	const_cast<float *>(glm::value_ptr(this->_projectionMatrix * this->_viewMatrix * this->_testModel->getModelMatrix())));
}

void			OpenGLModule2::_drawModelAtPosition(Model & model, int x, int y)
{
	glm::mat4	model_matrix = glm::mat4(1.0f);

	model_matrix = glm::translate(model_matrix, glm::vec3(x, y, 0));

	this->_shader->setTransformMatrix(
		const_cast<float *>(glm::value_ptr(this->_projectionMatrix * this->_viewMatrix * model_matrix)));

	model.draw();
}



void			OpenGLModule2::render(void)
{


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	// this->_renderCells();
	// glDrawArrays(GL_TRIANGLES, 0, 3);



	this->_updateMatrix();		// temp
	
	// YES THIS WORKS
	// this->_drawModelAtPosition(*this->_testModel, 0, 0);
	// this->_drawModelAtPosition(*this->_testModel, 0, 1);

	this->_testModel->draw();			// use this for testing

	// render border walls around the board

	this->_window.display();
}

void			OpenGLModule2::_renderCells(void)
{
	Cell *		cell;

	for (int x = 0; x < this->_board.getWidth(); x++)
		for (int y = 0; y < this->_board.getHeight(); y++)
			if ((cell = this->_board.getCell(x, y)))
				this->_renderCellAtPosition(*cell, x, y);
}

void			OpenGLModule2::_renderCellAtPosition(Cell & cell, int x, int y)
{
	(void)cell;
	(void)x;
	(void)y;

	// sf::RectangleShape	rect;
	// SnakeCell *			snakeCell;
	// FoodCell *			foodCell;
	// EnemyCell *			enemyCell;

	// rect.setSize(sf::Vector2f(CELL_WIDTH, CELL_WIDTH));
	// rect.setPosition(x * CELL_WIDTH, y * CELL_WIDTH);

	// if ((snakeCell = dynamic_cast<SnakeCell *>(&cell)))
	// 	this->_renderSnakeCell(*snakeCell, rect);
	// else if ((foodCell = dynamic_cast<FoodCell *>(&cell)))
	// 	this->_renderFoodCell(*foodCell, rect);
	// else if ((enemyCell = dynamic_cast<EnemyCell *>(&cell)))
	// 	this->_renderEnemyCell(*enemyCell, rect);
	// else
	// 	;	// some other type of cell?

}

// void		OpenGLModule2::_renderSnakeCell(SnakeCell & snakeCell, sf::RectangleShape & rect)
// {
// 	// Player dieded
// 	if (snakeCell.getSnake().isDead())
// 		rect.setFillColor(sf::Color(128, 128, 128));
// 	// Player 1
// 	else if (snakeCell.getSnake().getID() == 0)
// 	{
// 		rect.setFillColor(sf::Color::Cyan);
// 		if (snakeCell.isHead())
// 		{
// 			rect.setOutlineColor(sf::Color::Blue);
// 			rect.setOutlineThickness(20);
// 		}
// 	}
// 	// Player 2
// 	else
// 	{
// 		rect.setFillColor(sf::Color::Magenta);
// 		if (snakeCell.isHead())
// 		{
// 			rect.setOutlineColor(sf::Color::Red);
// 			rect.setOutlineThickness(20);
// 		}
// 	}

// 	this->_window.draw(rect);
// }

// void			OpenGLModule2::_renderFoodCell(FoodCell & foodCell, sf::RectangleShape & rect)
// {
// 	(void)foodCell;

// 	rect.setFillColor(sf::Color::Yellow);
// 	this->_window.draw(rect);
// }

// void			OpenGLModule2::_renderEnemyCell(EnemyCell & enemyCell, sf::RectangleShape & rect)
// {
// 	(void)enemyCell;

// 	rect.setFillColor(sf::Color(255, 165, 0));
// 	this->_window.draw(rect);
// }



























