#include "Shader.hpp"
#include "ResourceManager.hpp"
#include "NibblerException.hpp"

# define VERTEX_SHADER_FILE		"shaders/vertex_shader.glsl"
# define FRAGMENT_SHADER_FILE	"shaders/fragment_shader.glsl"

Shader::Shader(void)
{
	unsigned int	vertexShaderID;
	unsigned int	fragmentShaderID;
	std::string		vertexShaderCode = this->_readShaderFile(VERTEX_SHADER_FILE);
	std::string		fragmentShaderCode = this->_readShaderFile(FRAGMENT_SHADER_FILE);

	this->_compileShaderCode(vertexShaderID, GL_VERTEX_SHADER, vertexShaderCode.c_str());
	this->_compileShaderCode(fragmentShaderID, GL_FRAGMENT_SHADER, fragmentShaderCode.c_str());
	this->_linkShaderProgram(vertexShaderID, fragmentShaderID);
	glUseProgram(this->_shaderProgramID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

std::string				Shader::_readShaderFile(std::string filename)
{
	std::string			shaderCode;
	std::ifstream		shaderFile;
	std::stringstream	shaderStream;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(ResourceManager::getInstance().getAbsolutePathname(filename));
		shaderStream << shaderFile.rdbuf();
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		throw NibblerException("Shader::_readShaderFile() failed on " + filename);
	}
	return (shaderCode);
}


void		Shader::_compileShaderCode(unsigned int & shaderID, GLenum shaderType, const char * shaderCode)
{
	int		success;
	char	infoLog[LOG_SIZE];

	shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, LOG_SIZE, NULL, infoLog);
		throw NibblerException(std::string("glCompileShader() failed:\n") + std::string(infoLog));
	}
}

void		Shader::_linkShaderProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
	int		success;
	char	infoLog[LOG_SIZE];

	this->_shaderProgramID = glCreateProgram();
	glAttachShader(this->_shaderProgramID, vertexShader);
	glAttachShader(this->_shaderProgramID, fragmentShader);
	glLinkProgram(this->_shaderProgramID);

	glGetProgramiv(this->_shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
	    glGetProgramInfoLog(this->_shaderProgramID, LOG_SIZE, NULL, infoLog);
	    throw NibblerException(std::string("glLinkProgram() failed:\n") + std::string(infoLog));
	}
}

Shader::~Shader(void)
{
	glDeleteProgram(this->_shaderProgramID);
}



void				Shader::setMatrix(std::string name, const float * matrix)
{
	unsigned int	matrixLoc = glGetUniformLocation(this->_shaderProgramID, name.c_str());

	glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, matrix);
}

void				Shader::setVector3(std::string name, float * vector)
{
	unsigned int		vectorLoc = glGetUniformLocation(this->_shaderProgramID, name.c_str());

	glUniform3fv(vectorLoc, 1, vector);
}

void				Shader::setBool(std::string name, bool value)
{
	unsigned int	loc = glGetUniformLocation(this->_shaderProgramID, name.c_str());

	glUniform1i(loc, static_cast<int>(value));
}












