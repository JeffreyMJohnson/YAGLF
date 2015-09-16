#pragma once
#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"

#include <vector>
#include <iostream>

typedef unsigned int uint;
class RenderObject
{
public:
	enum Buffer_Type
	{
		VERTEX = GL_ELEMENT_ARRAY_BUFFER,
		INDEX = GL_ARRAY_BUFFER
	};

	enum Buffer_Usage
	{
		DYNAMIC_COPY = GL_DYNAMIC_COPY,
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
		DYNAMIC_READ = GL_DYNAMIC_READ,
		STATIC_COPY = GL_STATIC_COPY,
		STATIC_DRAW = GL_STATIC_DRAW,
		STATIC_READ = GL_STATIC_READ
	};

	bool LoadBuffer(Buffer_Type type, Buffer_Usage usage, const std::vector<float>& data);
private:
	uint mVAO, mVBO, mIBO;

};