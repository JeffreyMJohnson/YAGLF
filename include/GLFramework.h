#pragma once
#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "glm\vec4.hpp"
#include "Shader.h"
#include "Geometry.h"
#include "input\Keyboard.h"
#include "tiny_obj_loader\tiny_obj_loader.h"
#include "fbx_loader\FBXFile.h"
#include "stb\stb_image.h"

#include <iostream>
#include <map>
#include <vector>
#include <assert.h>

/*
	TODO:
		implement draw call allowing user to choose to draw object or not each frame.
		get user access to shaders out, making shaders private to framework. can have the shader flexible enough
			to handle scenarios.

*/

/*
RenderObject
	vao
	ibo
	tricount


*/

using glm::vec2;
using glm::vec3;
using glm::vec4;

struct RenderObject
{
	uint vao = 0, vbo = 0, ibo = 0, indexCount = 0;
};



struct Texture
{
	/*
	Base internal format
	note:Add more formats here as needed in future.
	*/
	enum Format
	{
		RGB8 = GL_RGB8,
		RGB32F = GL_RGB32F,
		DEPTH = GL_DEPTH_COMPONENT
	};
	uint width = 1;
	uint height = 1;
	Format format;
	uint handle = 0;
};

class Timer
{
	float mTotalTime = 0;
	float mDeltaTime = 0;

public:
	void Update()
	{
		float temp = mTotalTime;
		mTotalTime = glfwGetTime();
		mDeltaTime = mTotalTime - temp;
	}
	float GetTotalTime() { return mTotalTime; }
	float GetDeltaTime() { return mDeltaTime; }
};

struct Window
{
	int height = 0;
	int width = 0;
	const char* title;
	GLFWwindow* handle = nullptr;
	vec4 clearColor;
};

const vec4 WHITE(1, 1, 1, 1);
const vec4 GREY(.5f, .5f, .5f, 1);
const vec4 RED(1, 0, 0, 1);
const vec4 GREEN(0, 1, 0, 1);

class GLFramework
{
public:
	/*
	ID for preset models
	*/
	enum GeometryModel
	{
		CUBE = 0
	};

	/*
	Initializes OpenGL and creates a window od given height and width.
	This should be first Method called to initialize framework.
	*/
	static bool Startup(const int width, const int height, const char* title, const vec4 clearColor);

	/*
	Wrapper method for glfwwindowshouldclose() method.
	*/
	static bool WindowShouldClose() { return glfwWindowShouldClose(sWindow->handle); }
	
	static uint CreateQuad();

	/*
	Loads shader code from given path and attempts to compile and link to shader program.  
	Returns unsigned int greater than 0 if successful, else returns 0 which means error occured, check
	console for error logging.
	*/
	static uint LoadShader(std::string vertexPath, std::string fragmentPath);

	/*
	Sets current bound shader program to given.
	*/
	static void UseShader(uint shader);

	/*
	Set uniform variable of given shader.
	shader - ID returned by LoadShader
	varName - string of uniform variable in shader to assign
	type - Shader::UniformType enum of uniform variables type
	value - pointer to value setting the uniform to.
	count - Only used with setting type of texture2d, this is the texture unit to bind the texture to.
	*/
	static void SetShaderUniform(const unsigned int shader, std::string varName, const Shader::UniformType type, const void* value, const uint count = 0);

	//DEBUG ONLY NEEDS TO BE REFACTORED OUT
	static unsigned int GetShaderProgram(const unsigned int shader) { return sShaders[shader]->GetProgram(); }

	//static bool SetShader(const char* vertexPath, const char* fragmentPath);
	//static void SetShaderUniform(const char* name, const Shader::UniformType type, const void* value);
	
	static uint LoadTexture(const char * path);

	static void SetWireframe(bool value);
	
	static uint LoadModel(const char* path);
	static uint LoadModel(Geometry& geometry);


	/*
	draw geometry, wraps glDrawElements
	*/
	static void DrawModel(uint modelID);

	static float GetDeltaTime() { return sTimer.GetDeltaTime(); }
	static float GetTotalTime() { return sTimer.GetTotalTime(); }

	static bool Update();
	static vec4 GetClearColor();
	static void SetClearColor(const vec4 color);
	static void Cleanup();

	static int counter;

private:

	static Window* sWindow;
	//static Shader* sShader;
	static std::vector<Shader*> sShaders;
	static std::vector<uint> sTextures;
	static std::vector<RenderObject> sRenderObjects;
	static bool useWireframe;
	static Timer sTimer;

	static uint LoadObject();
	//static bool LoadBuffers(const Geometry& geometry);
	//static void UpdateFlyCamControls();
	static Geometry BuildQuad();

	static uint MakeVAO(Geometry& geometry);
	static void DrawVAO(uint renderObject);
	static void KillVAO(uint renderObject);


};