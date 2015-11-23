#pragma once
#define GLM_SWIZZLE

#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

#include "Geometry.h"
#include "Shader.h"

#include <iostream>
#include <string>
#include <assert.h>
#include <vector>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

struct Window
{
	int height = 0;
	int width = 0;
	const char* title;
	GLFWwindow* handle = nullptr;
};

struct Mesh
{
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ibo = 0;
	GLuint count;
};

struct GameObject
{
	Mesh mesh;
};

struct FrameBuffer
{

};

class Dryv3r
{
public:
	/*
	Initialize the framework and create a window with given size and title
	*/
	static bool Init(const int width, const int height, const char* title);

	/*
	Call once per frame
	*/
	static bool Update();

	static void Cleanup();

	/*
	Call after updating everything in frame and ready to send to screen.

	*/
	static void Draw();

	/*
	Call when want to draw the given object to the screen this frame.
	*/
	static void DrawGameObject(GameObject& gameObject);
	
	/*
	returns reference to 3D cube game object
	*/
	static GameObject& GetCube() { return mCube; }
	
	/*
	returns ref to simple 2d triangle in screenspace for early testing
	*/
	static GameObject& GetTestTri() { return mTestTri; }
private:
	
	static Window mWindow;
	static GameObject mCube;
	static GameObject mTestTri;
	static std::vector<GameObject*> mObjectsToDraw;
	static Shader mShader;
	//todo::need to devise system for handing out gameobjects to user

	static Mesh LoadMesh(Geometry& geometry);
	static void DrawMesh(Mesh& mesh);
	
};
