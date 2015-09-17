#pragma once
#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "glm\vec4.hpp"
#include "glm\vec2.hpp"

#include <vector>
#include <iostream>

typedef unsigned int uint;



struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
	//glm::vec4 normal;
	//glm::vec2 uv;
};

struct Geometry
{
	Vertex* vertices;
	uint verticesSize;
	uint* indeces;
	uint indecesSize;
};

struct RenderObject
{
	uint vao = 0, vbo = 0, ibo = 0, size = 0;

	void LoadBuffers(Geometry& geometry);
};


//class RenderObject
//{
//public:
//	enum Buffer_Type
//	{
//		VERTEX = GL_ELEMENT_ARRAY_BUFFER,
//		INDEX = GL_ARRAY_BUFFER
//	};
//
//	enum Buffer_Usage
//	{
//		DYNAMIC_COPY = GL_DYNAMIC_COPY,
//		DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
//		DYNAMIC_READ = GL_DYNAMIC_READ,
//		STATIC_COPY = GL_STATIC_COPY,
//		STATIC_DRAW = GL_STATIC_DRAW,
//		STATIC_READ = GL_STATIC_READ
//	};
//
//	//RenderObject();
//
//	bool LoadBuffer(Buffer_Type type, Buffer_Usage usage, const void* data, const uint dataSize, uint indexCount);
//	void Draw();
//	const uint GetVBO() { return mVBO; }
//	const uint GetVAO() { return mVAO; }
//	const uint GetIBO() { return mIBO; }
//	const uint GetIndexCount() { return mIndexCount; }
//private:
//	uint mVAO, mVBO, mIBO, mIndexCount;
//
//};