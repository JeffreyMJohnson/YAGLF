#include "Shader.h"
#include <assert.h>

const bool Shader::LoadShader(const std::string vertexPath, const std::string fragmentPath)
{

	uint vertex = LoadSubShader(GL_VERTEX_SHADER, vertexPath);
	uint fragment = LoadSubShader(GL_FRAGMENT_SHADER, fragmentPath);

	int success = GL_FALSE;
	mProgram = glCreateProgram();
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);
	glLinkProgram(mProgram);
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetProgramInfoLog(mProgram, length, 0, log);
		assert(false && "Error linking shader:" && log);
		delete[] log;
		return false;
	}
	return true;
}

void Shader::FreeShader()
{
	glDeleteProgram(mProgram);
}

void Shader::SetUniform(const char * name, const UniformType type, const void * value, const uint count)
{
	GLint location = glGetUniformLocation(mProgram, name);
	switch (type)
	{
	case MAT4:
		glUniformMatrix4fv(location, 1, false, (const GLfloat*)value);
		break;
	case VEC4:
		glUniform4fv(location, 1, (GLfloat*)value);
		break;
	case VEC3:
		glUniform3fv(location, 1, (GLfloat*)value);
		break;
	case FLO1:
		glUniform1f(location, *(GLfloat*)value);
		break;
	case INT1:
		glUniform1i(location, *(GLint*)value);
		break;
	case UINT1:
		glUniform1ui(location, *(GLuint*)value);
		break;
	case TEXTURE2D:
		glUniform1i(location, count);
		glActiveTexture(GL_TEXTURE0 + count);
		glBindTexture(GL_TEXTURE_2D, *(GLuint*)value);
		break;
	case BOOL:
		glUniform1i(location, *(GLboolean*)value);
		break;
	default:
		assert(false && "should bad uniform type.");
	}
}

uint Shader::LoadSubShader(uint shaderType, const std::string path)
{
	std::ifstream stream(path);
	std::string contents = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
	if (contents.size() == 0)
	{
		std::cerr << "Nothing in file " << path << std::endl;
		assert(false);
		return 0;
	}
	char* code = new char[contents.length() + 1];
	strncpy_s(code, contents.length() + 1, contents.c_str(), contents.length());

	uint shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &code, 0);
	GLint success = GL_FALSE;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetShaderInfoLog(shader, length, 0, log);
		std::cerr << log << std::endl;
		delete[] log;
		glDeleteShader(shader);


		assert(false && "Error compiling shader.");
		return 0;
	}

	delete[] code;

	return shader;
}
