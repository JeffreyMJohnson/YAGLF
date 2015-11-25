#pragma once
#include "glm\vec4.hpp"
#include "glm\vec2.hpp"

#include <vector>
#include <iostream>


struct Vertex
{
	glm::vec4 position;
	glm::vec4 normal;
	glm::vec4 tangent;
	glm::vec2 UV;
};

struct Geometry
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

const glm::vec4 UP(0, 1, 0, 0);
const glm::vec4 DOWN = -UP;
const glm::vec4 RIGHT(1, 0, 0, 0);
const glm::vec4 LEFT = -RIGHT;
const glm::vec4 FORWARD(0, 0, 1, 0);
const glm::vec4 BACKWARD = -FORWARD;


static Geometry BuildCube()
{
	std::vector<Vertex> cubeVerts;
	cubeVerts.resize(24);
// Bottom
	cubeVerts[0] = { glm::vec4(-1,-1, 1, 1), DOWN, RIGHT, glm::vec2(0,1) };
	cubeVerts[1] = { glm::vec4(1,-1, 1, 1), DOWN, RIGHT, glm::vec2(1,1) };
	cubeVerts[2] = { glm::vec4(1,-1,-1, 1), DOWN, RIGHT, glm::vec2(1,0) };
	cubeVerts[3] = { glm::vec4(-1,-1,-1, 1), DOWN, RIGHT, glm::vec2(0,0) };
// Top
	cubeVerts[4] = { glm::vec4(-1, 1, 1, 1), UP, RIGHT, glm::vec2(0,1) };
	cubeVerts[5] = { glm::vec4(1, 1, 1, 1), UP, RIGHT, glm::vec2(1,1) };
	cubeVerts[6] = { glm::vec4(1, 1,-1, 1), UP, RIGHT, glm::vec2(1,0) };
	cubeVerts[7] = { glm::vec4(-1, 1,-1, 1), UP, RIGHT, glm::vec2(0,0) };

	// back
	cubeVerts[8] = { glm::vec4(-1, 1, -1, 1), BACKWARD, LEFT, glm::vec2(0, 1) };
	cubeVerts[9] = { glm::vec4(1, 1, -1, 1), BACKWARD, LEFT, glm::vec2(1,1) };
	cubeVerts[10] = { glm::vec4(1, -1,-1, 1), BACKWARD, LEFT, glm::vec2(1,0) };
	cubeVerts[11] = { glm::vec4(-1, -1,-1, 1), BACKWARD, LEFT, glm::vec2(0,0) };

	// front
	cubeVerts[12] = { glm::vec4(-1, 1, 1, 1), FORWARD, RIGHT, glm::vec2(0, 1) };
	cubeVerts[13] = { glm::vec4(1, 1, 1, 1), FORWARD,  RIGHT, glm::vec2(1,1) };
	cubeVerts[14] = { glm::vec4(1, -1,1, 1), FORWARD,  RIGHT, glm::vec2(1,0) };
	cubeVerts[15] = { glm::vec4(-1, -1,1, 1), FORWARD, RIGHT, glm::vec2(0,0) };

	// left
	cubeVerts[16] = { glm::vec4(-1, 1, -1, 1), LEFT, FORWARD, glm::vec2(1, 0) };
	cubeVerts[17] = { glm::vec4(-1, 1, 1, 1), LEFT, FORWARD, glm::vec2(1,1) };
	cubeVerts[18] = { glm::vec4(-1,-1, 1, 1), LEFT, FORWARD, glm::vec2(0,1) };
	cubeVerts[19] = { glm::vec4(-1,-1,-1, 1), LEFT, FORWARD, glm::vec2(0,0) };
		// right
	cubeVerts[20] = { glm::vec4(1, 1,-1, 1), RIGHT, BACKWARD, glm::vec2(1,0) };
	cubeVerts[21] = { glm::vec4(1, 1, 1, 1), RIGHT, BACKWARD, glm::vec2(1,1) };
	cubeVerts[22] = { glm::vec4(1,-1, 1, 1), RIGHT, BACKWARD, glm::vec2(0,1) };
	cubeVerts[23] = { glm::vec4(1,-1,-1, 1), RIGHT, BACKWARD, glm::vec2(0,0) };

	unsigned int CubeTris[] = {
		3,2,0,0,2,1,
		7,4,6,6,4,5,
		11,8,10,10,8,9,
		15, 14, 12, 12, 14, 13,
		19,18,16,16,18,17,
		23,20,22,22,20,21 };
	unsigned dataArraySize = sizeof(CubeTris) / sizeof(unsigned int);

	Geometry cube;
	cube.vertices = cubeVerts;
	cube.indices.insert(cube.indices.end(), &CubeTris[0], &CubeTris[dataArraySize]);
	return cube;
}

static Geometry BuildTestTri()
{
	Geometry testTri;
	std::vector<Vertex> vertices;
	vertices.resize(3);
	Vertex v0;
	v0.position = glm::vec4(.25f, -.25f, .5f, 1.0f);
	v0.UV = glm::vec2(0);
	vertices[0] = v0;
	Vertex v1;
	v1.position = glm::vec4(-.25f, -.25f, .5f, 1.0f);
	v1.UV = glm::vec2(0, 1);
	vertices[1] = v1;
	Vertex v2;
	v2.position = glm::vec4(.25f, .25f, .5f, 1.0f);
	v2.UV = glm::vec2(1);
	vertices[2] = v2;
	std::vector<unsigned int> indices{ 0,1,2 };
	testTri.vertices = vertices;
	testTri.indices = indices;
	return testTri;
}

/*
const Vertex QuadVerts[] =
{
	{ vec4(-1, 1, 0, 1), BACKWARD,  LEFT, vec2(0,1) },
	{ vec4(1, 1, 0, 1), BACKWARD,  LEFT, vec2(1,1) },
	{ vec4(1,-1, 0, 1), BACKWARD,  LEFT, vec2(1,0) },
	{ vec4(-1,-1, 0, 1), BACKWARD,  LEFT, vec2(0,0) },
};

const unsigned QuadTris[] = { 3,1,0, 3,2,1 };
*/