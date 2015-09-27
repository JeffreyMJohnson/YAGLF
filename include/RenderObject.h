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
	glm::vec4 normal;
	glm::vec4 tangent;
	glm::vec2 UV;
};

struct Geometry
{
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
};

struct RenderObject
{
	uint vao = 0, vbo = 0, ibo = 0, indexCount = 0;

	void LoadBuffers(Geometry& geometry);
	void Draw();
	void DeleteBuffers();
};