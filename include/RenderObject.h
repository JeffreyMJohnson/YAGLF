#pragma once
#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "glm\vec4.hpp"
#include "glm\vec2.hpp"

#include <vector>
#include <iostream>

typedef unsigned int uint;
using glm::vec2;

const vec4 up(0, 1, 0, 0);
const vec4 down = -up;
const vec4 right(1, 0, 0, 0);
const vec4 left = -right;
const vec4 forward(0, 0, 1, 0);
const vec4 backward = -forward;

const Vertex QuadVerts[] =
{
	{ vec4(-1, 1, 0, 1), vec4(0), backward,  left, vec2(0,1) },
	{ vec4(1, 1, 0, 1), vec4(0), backward,  left, vec2(1,1) },
	{ vec4(1,-1, 0, 1), vec4(0), backward,  left, vec2(1,0) },
	{ vec4(-1,-1, 0, 1), vec4(0), backward,  left, vec2(0,0) },
};

const unsigned QuadTris[] = { 3,1,0, 3,2,1 };

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