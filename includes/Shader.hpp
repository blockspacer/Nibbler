#ifndef SHADER_HPP
# define SHADER_HPP

# define LOG_SIZE				512

# include <string>
# include <fstream>
# include <sstream>
# include <GL/glew.h>

class Shader
{
public:

	Shader(void);
	~Shader(void);

	void			setMatrix(std::string name, const float * matrix);
	void			setVector3(std::string name, float * vector);
	void			setBool(std::string name, bool value);

private:

	unsigned int	_shaderProgramID;

	Shader(const Shader & src);
	Shader & operator=(const Shader & rhs);

	std::string		_readShaderFile(std::string filename);
	void			_compileShaderCode(unsigned int & shaderID, GLenum shaderType, const char * shaderCode);
	void			_linkShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);

};

#endif
