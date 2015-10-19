#pragma once
#include <string>
#include <fstream>
#include <iostream>


#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"


typedef unsigned int uint;

class Shader
{
public:
	enum UniformType
	{
		MAT4,
		VEC4,
		VEC3,
		FLO1, 
		INT1, 
		UINT1,
		TEXTURE2D
	};

	const bool LoadShader(const std::string vertexPath, const std::string fragmentPath);
	void FreeShader();
	const uint GetProgram() { return mProgram; }
	void SetUniform(const char* name, const UniformType type, const void* value, const uint count = 0);
private:
	uint mProgram = 0;

	uint LoadSubShader(uint shaderType, const std::string path);

};