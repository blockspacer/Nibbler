#include "OpenGLModule.hpp"
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
#include "Shader.hpp"
#include <iostream>

extern "C"
{
	IModule *	createOpenGLModule(Nibbler & nibbler, Board & board, std::string title)
	{
		return (new OpenGLModule(nibbler, board, title));
	}
}

OpenGLModule::OpenGLModule(Nibbler & nibbler, Board & board, std::string title) :
	_nibbler(nibbler),
	_board(board)
{
	this->_initOpenGLStuff(title);
	this->_shader = new Shader();
	this->_initModels();
	this->_initLightPosition();
	this->_resetGraphicsParameters();
}

void			OpenGLModule::_initOpenGLStuff(std::string & title)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
		throw NibblerException("SDL_Init() failed");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1);

	if (!(this->_window = SDL_CreateWindow(title.c_str(), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL)))
		throw NibblerException("SDL_CreateWindow() failed");
	this->_context = SDL_GL_CreateContext(this->_window);

	glewExperimental = GL_TRUE;		// needed for MAC OSX

	GLenum err = glewInit();

	if (err != GLEW_OK)
		throw NibblerException("glewInit() failed:\n" +
			std::string(reinterpret_cast<const char *>(glewGetErrorString(err))));

	glEnable(GL_DEPTH_TEST);
}

void			OpenGLModule::_initModels(void)
{
	glm::mat4	tempMatrix;

	float		floorScaleX = this->_board.getWidth() + 2.0f;
	float		floorScaleY = this->_board.getHeight() + 2.0f;

	this->_floorModel = new Model("models/square.obj", "models/doge_rainbow.jpg");
	this->_floorModel->setScaleMatrix(glm::scale(glm::mat4(1.0f),
		glm::vec3(floorScaleX, floorScaleY, 1.0f)));
	this->_floorModel->setTranslationMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, -0.5f)));

	this->_wallModel = new Model("models/cube.obj", "models/wall.jpg");
	this->_wallModel->setRotationMatrix(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	
	this->_snakeHeadModel1 = new Model("models/finn.obj", "models/finn.png");
	this->_snakeHeadModel1->setRotationMatrix(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

	this->_snakeHeadModel2 = new Model("models/cirno.obj", "models/cirno.png");
	tempMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	tempMatrix = glm::rotate(tempMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->_snakeHeadModel2->setRotationMatrix(tempMatrix);

	this->_snakeBodyModel1 = new Model("models/teapot.obj", "models/finn.png");
	tempMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	tempMatrix = glm::rotate(tempMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->_snakeBodyModel1->setRotationMatrix(tempMatrix);

	this->_snakeBodyModel2 = new Model("models/icosahedron.obj", "models/cirno.png");
	tempMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	tempMatrix = glm::rotate(tempMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->_snakeBodyModel2->setRotationMatrix(tempMatrix);

	this->_snakeDeadModel = new Model("models/skeleton_sit.obj", "models/skeleton.jpg");
	this->_snakeDeadModel->setRotationMatrix(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	this->_foodModel = new Model("models/pizza.obj", "models/pizza.png");
	this->_foodModel->setSpinning();

	this->_enemyModel = new Model("models/hero_dog.obj", "models/hero_dog.png");
	this->_enemyModel->setRotationMatrix(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
}

void				OpenGLModule::_initLightPosition(void)
{
	float			xCenter = this->_board.getWidth() / 2.0f;
	float			yCenter = -this->_board.getHeight() / 2.0f;

	int		largestSide = std::max(this->_board.getWidth(), this->_board.getHeight());
	float	zDist = largestSide / atan(glm::radians(45.0f));
	zDist /= 8.0f;

	this->_lightPosition = glm::vec3(xCenter, yCenter, -zDist);
	this->_shader->setVector3("light_position", glm::value_ptr(this->_lightPosition));
}

void			OpenGLModule::_resetGraphicsParameters(void)
{
	this->_isWireframe = false;
	glPolygonMode(GL_FRONT_AND_BACK, this->_isWireframe ? GL_LINE : GL_FILL);

	this->_isShaded = false;
	this->_shader->setBool("is_shaded", this->_isShaded);

	this->_isFirstPersonView = false;
}

OpenGLModule::~OpenGLModule(void)
{
	printf("~OpenGLModule()\n");

	delete this->_floorModel;
	delete this->_wallModel;
	delete this->_snakeHeadModel1;
	delete this->_snakeHeadModel2;
	delete this->_snakeBodyModel1;
	delete this->_snakeBodyModel2;
	delete this->_snakeDeadModel;
	delete this->_foodModel;
	delete this->_enemyModel;

	delete this->_shader;

	SDL_DestroyWindow(this->_window);
	SDL_GL_DeleteContext(this->_context);
	
	SDL_Quit();
}

void			OpenGLModule::disable(void)
{
	// printf("OpenGLModule: disable()\n");
	SDL_HideWindow(this->_window);
}

void			OpenGLModule::enable(void)
{
	// printf("OpenGLModule: enable()\n");
	SDL_GL_MakeCurrent(this->_window, this->_context);
	SDL_ShowWindow(this->_window);
}

void			OpenGLModule::handleEvents(void)
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

void			OpenGLModule::_handleKeyPressEvent(SDL_Event & event)
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
		case SDLK_KP_0:
			this->_resetGraphicsParameters();
			return;
		case SDLK_w:
			this->_toggleWireframeMode();
			return;
		case SDLK_s:
			this->_toggleShading();
			return;
		case SDLK_v:
			this->_toggleView();
			return;
		default:
			break;
	}
}

void			OpenGLModule::_toggleWireframeMode(void)
{
	this->_isWireframe = !this->_isWireframe;
	glPolygonMode(GL_FRONT_AND_BACK, this->_isWireframe ? GL_LINE : GL_FILL);
}

void			OpenGLModule::_toggleShading(void)
{
	this->_isShaded = !this->_isShaded;
	this->_shader->setBool("is_shaded", this->_isShaded);
}

void			OpenGLModule::_toggleView(void)
{
	this->_isFirstPersonView = !this->_isFirstPersonView;
}

void			OpenGLModule::_drawModelAtPosition(Model & model, int x, int y)
{
	this->_shader->setMatrix("projection_matrix", glm::value_ptr(this->_projectionMatrix));
	this->_shader->setMatrix("view_matrix", glm::value_ptr(this->_viewMatrix));

	this->_shader->setMatrix("model_scale_matrix", value_ptr(model.getScaleMatrix()));
	this->_shader->setMatrix("model_rotate_matrix", value_ptr(model.getRotationMatrix()));
	this->_shader->setMatrix("model_translate_matrix", value_ptr(
		glm::translate(glm::mat4(1.0f), glm::vec3(x, -y, 0)) * model.getTranslationMatrix()));

	model.draw();
}

void			OpenGLModule::_drawModelAtPositionFacing(Model & model, int x, int y, e_direction direction)
{
	glm::mat4	rotationMatrix;

	switch (direction)
	{
		case EAST:
			rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		case SOUTH:
			rotationMatrix = glm::mat4(1.0f);
			break;
		case WEST:
			rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		case NORTH:
			rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		default:
			rotationMatrix = glm::mat4(1.0f);
			break;
	}

	this->_shader->setMatrix("projection_matrix", glm::value_ptr(this->_projectionMatrix));
	this->_shader->setMatrix("view_matrix", glm::value_ptr(this->_viewMatrix));

	this->_shader->setMatrix("model_scale_matrix", value_ptr(model.getScaleMatrix()));
	this->_shader->setMatrix("model_rotate_matrix", value_ptr(rotationMatrix * model.getRotationMatrix()));
	this->_shader->setMatrix("model_translate_matrix", value_ptr(
		glm::translate(glm::mat4(1.0f), glm::vec3(x, -y, 0)) * model.getTranslationMatrix()));

	model.draw();
}

void			OpenGLModule::render(void)
{
	Cell *		cell;

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->_updateViewMatrix();
	this->_renderFloorAndWalls();

	for (int x = 0; x < this->_board.getWidth(); x++)
		for (int y = 0; y < this->_board.getHeight(); y++)
			if ((cell = this->_board.getCell(x, y)))
				this->_renderCell(*cell);

	SDL_GL_SwapWindow(this->_window);
}

void				OpenGLModule::_updateViewMatrix(void)
{
	static int		largestSide = std::max(this->_board.getWidth(), this->_board.getHeight());
	static float	xCenter = this->_board.getWidth() / 2.0f - 0.5f;
	static float	yCenter = -(this->_board.getHeight() / 2.0f - 0.5f);

	static float	zDist = largestSide / atan(glm::radians(45.0f));

	// lock on player 1's view point
	if (this->_isFirstPersonView)
	{
		SnakeCell &	headCell = this->_nibbler.getActivePlayersSnakeHeadCell();
		int			x = headCell.getX();
		int			y = headCell.getY();
		int			offset = 4;
		glm::mat4	rotationMatrix;

		switch (headCell.getDirection())
		{
			case EAST:
				rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				x = x - offset;
				break;
			case SOUTH:
				rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				y = y - offset;
				break;
			case WEST:
				rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				x = x + offset;
				break;
			case NORTH:
				rotationMatrix = glm::mat4(1.0f);
				y = y + offset;
				break;
			default:
				rotationMatrix = glm::mat4(1.0f);
				break;
		}
		this->_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-x, y, -3));
		this->_viewMatrix = rotationMatrix * this->_viewMatrix;

		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		this->_viewMatrix = rotationMatrix * this->_viewMatrix;
		this->_projectionMatrix = glm::perspective(glm::radians(80.0f), ASPECT_RATIO, 0.1f, 100.0f);
	}
	else
	{
		this->_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-xCenter, -yCenter, -zDist));
		this->_projectionMatrix = glm::perspective(glm::radians(45.0f), ASPECT_RATIO, 0.1f, 100.0f);
	}
}

void			OpenGLModule::_renderFloorAndWalls(void)
{
	static float	floorX = this->_board.getWidth() / 2.0f;
	static float	floorY = this->_board.getHeight() / 2.0f;

	// draw floor
	this->_drawModelAtPosition(*this->_floorModel, floorX, floorY);

	// draw walls
	for (int x = -1; x <= this->_board.getWidth(); x++)
	{
		this->_drawModelAtPosition(*this->_wallModel, x, -1);
		this->_drawModelAtPosition(*this->_wallModel, x, this->_board.getHeight());
	}
	for (int y = 0; y < this->_board.getHeight(); y++)
	{
		this->_drawModelAtPosition(*this->_wallModel, -1, y);
		this->_drawModelAtPosition(*this->_wallModel, this->_board.getWidth(), y);
	}
}

void			OpenGLModule::_renderCell(Cell & cell)
{
	try
	{
		this->_renderSnakeCell(dynamic_cast<SnakeCell &>(cell));
	}
	catch (std::bad_cast& bc)
	{
		try
		{
			this->_renderFoodCell(dynamic_cast<FoodCell &>(cell));
		}
		catch (std::bad_cast& bc)
		{
			this->_renderEnemyCell(dynamic_cast<EnemyCell &>(cell));
		}
	}
}

void			OpenGLModule::_renderSnakeCell(SnakeCell & snakeCell)
{
	Model *		model;

	// Player dieded
	if (snakeCell.getSnake().isDead())
		model = this->_snakeDeadModel;
	// Player 1
	else if (snakeCell.getSnake().getID() == 0)
	{
		if (snakeCell.isHead())
			model = this->_snakeHeadModel1;
		else
			model = this->_snakeBodyModel1;
	}
	// Player 2
	else
	{
		if (snakeCell.isHead())
			model = this->_snakeHeadModel2;
		else
			model = this->_snakeBodyModel2;
	}
	this->_drawModelAtPositionFacing(*model, snakeCell.getX(), snakeCell.getY(), snakeCell.getDirection());
}

void			OpenGLModule::_renderFoodCell(FoodCell & foodCell)
{
	this->_drawModelAtPosition(*this->_foodModel, foodCell.getX(), foodCell.getY());
}

void			OpenGLModule::_renderEnemyCell(EnemyCell & enemyCell)
{
	this->_drawModelAtPositionFacing(*this->_enemyModel, enemyCell.getX(), enemyCell.getY(), enemyCell.getDirection());
}



























