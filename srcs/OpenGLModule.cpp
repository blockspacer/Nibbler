#include "OpenGLModule.hpp"
#include "ResourceManager.hpp"
#include "NibblerException.hpp"
#include "Nibbler.hpp"
#include "Board.hpp"
#include "Cell.hpp"

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	Nibbler *	nibbler  = static_cast<Nibbler *>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
				nibbler->terminate();
				break;
			case GLFW_KEY_1:
				nibbler->selectModule1();
				break;
			case GLFW_KEY_2:
				nibbler->selectModule2();
				break;
			case GLFW_KEY_3:
				nibbler->selectModule3();
				break;
			case GLFW_KEY_LEFT:
				nibbler->turnLeftP1();
				break;
			case GLFW_KEY_RIGHT:
				nibbler->turnRightP1();
				break;
			case GLFW_KEY_KP_4:
				nibbler->turnLeftP2();
				break;
			case GLFW_KEY_KP_6:
				nibbler->turnRightP2();
				break;
			case GLFW_KEY_R:
				nibbler->startNewRound();
				break;


			// DEBUG

			case GLFW_KEY_TAB:
				printf("OpenGL Module: pressed TAB\n");
				nibbler->selectNextModule();
				break;

			case GLFW_KEY_SPACE:
				nibbler->_update();
				break;
			case GLFW_KEY_D:
				printf("OpenGL Module: pressed D\n");
				break;



			default:
				break;
		}
	}

	(void)scancode;
	(void)action;
	(void)mods;
}

float vertices[] =
{
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};


OpenGLModule::OpenGLModule(Nibbler & nibbler, Board & board) :
	_nibbler(nibbler), _board(board)
{
	this->_initOpenGLStuff();
	this->_shader = new Shader();
	this->_initVertexObjects();

}

void			OpenGLModule::_initOpenGLStuff(void)
{
	if (glfwInit() == GLFW_FALSE)
		throw NibblerException("glfwInit() failed");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	if (!(this->_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL", NULL, NULL)))
		throw NibblerException("glfwCreateWindow() failed");

	glfwMakeContextCurrent(this->_window);
	
	if (glewInit() != GLEW_OK)
		throw NibblerException("glewInit() failed");

	glfwSetWindowPos(this->_window, 0, 0);
	glfwSetWindowAspectRatio(this->_window, SCREEN_WIDTH, SCREEN_HEIGHT);
	glfwSetWindowUserPointer(this->_window, &this->_nibbler);
	glfwSetKeyCallback(this->_window, key_callback);
}

# define STRIDE_LEN 3

void			OpenGLModule::_initVertexObjects(void)
{
	unsigned int	vbo;
	unsigned int	vao;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(float) * STRIDE_LEN * 3 * m->obj.triangle_count, m->gl_data, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE_LEN * sizeof(float), (void *)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	
}

OpenGLModule::~OpenGLModule(void)
{
	printf("~OpenGLModule()\n");
	delete this->_shader;
	
	glfwDestroyWindow(this->_window);
	// glfwTerminate();
}

void			OpenGLModule::disable(void)
{
	glfwHideWindow(this->_window);
}

void			OpenGLModule::enable(void)
{
	glfwMakeContextCurrent(this->_window);
	glfwShowWindow(this->_window);
}

void			OpenGLModule::handleEvents(void)
{
	// handles when the icon X to close window is pressed
	if (glfwWindowShouldClose(this->_window))
		this->_nibbler.terminate();
}

void			OpenGLModule::render(void)
{
	// printf("GOT HERE 1\n");

	// clear
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// printf("GOT HERE 2\n");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// printf("GOT HERE 3\n");


	// this->_renderCells();

	glDrawArrays(GL_TRIANGLES, 0, 3);


	// printf("GOT HERE 4\n");


	// display
	glfwSwapBuffers(this->_window);

	// printf("GOT HERE 5\n");

	glfwPollEvents();

	// printf("GOT HERE 6\n");
}

void			OpenGLModule::_renderCells(void)
{
	Cell *		cell;

	for (int x = 0; x < this->_board.getWidth(); x++)
		for (int y = 0; y < this->_board.getHeight(); y++)
			if ((cell = this->_board.getCell(x, y)))
				this->_renderCellAtPosition(*cell, x, y);
}

void			OpenGLModule::_renderCellAtPosition(Cell & cell, int x, int y)
{
	(void)cell;
	(void)x;
	(void)y;
}


