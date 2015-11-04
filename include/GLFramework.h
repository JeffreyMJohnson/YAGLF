#pragma once
#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "glm\vec4.hpp"
#include "camera\Camera.h"
#include "Shader.h"
#include "RenderObject.h"
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

using glm::vec2;
using glm::vec3;
using glm::vec4;



struct Color
{
	float r = 0, g = 0, b = 0, a = 0;
	Color() {};
	Color(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	vec4 GetVector4()
	{
		return vec4(r, g, b, a);
	}
};

struct Material
{
	vec4 ambient, diffuse, specular;
	float specularPower;
};

struct BaseLight
{
	Color color;
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



struct DirectionalLight : public BaseLight
{
	vec3 direction;
	DirectionalLight(vec3 a_direction, Color a_color)
	{
		direction = a_direction;
		color = a_color;
	}
};

struct PointLight
{
	Color ambient, diffuse, specular;
	vec3 position;
	float falloff;
	PointLight(const Color ambient, const Color diffuse, const Color specular, const glm::vec3 position, float falloff)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->position = position;
		this->falloff = falloff;
	}
};

struct Window
{
	int height = 0;
	int width = 0;
	const char* title;
	GLFWwindow* handle = nullptr;
	Color clearColor;
};

const Color WHITE(1, 1, 1, 1);
const Color GREY(.5f, .5f, .5f, 1);
const Color RED(1, 0, 0, 1);
const Color GREEN(0, 1, 0, 1);

class GLFramework
{
public:

	/*
	Initializes OpenGL and creates a window od given height and width.
	This should be first Method called to initialize framework.
	*/
	static bool Startup(const int width, const int height, const char* title, const Color clearColor);

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
	
	static bool SetCameraView(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up);
	static bool SetCameraProjection(const float fov, const float aspectRatio, const float a_near, const float a_far);
	static vec3 GetCameraPosition() { return sCamera->GetPosition(); }
	static void SlideCamera(const float hDistance, const float vDistance);
	static void MoveCamera(const float distance);

	static uint LoadModel(const char* path);
	static uint LoadModel(Geometry& geometry);

	static uint SetDirectionalLight(const Color color, const vec3 direction);
	static void SetLightDirection(const uint light, const vec3 newDirection);
	//static uint CreateLight(const vec3 position, const Color ambientColor, const Color diffuseColor, const Color specularColor);
	//static Light& GetLight(const uint light) { return sLights[light]; }

	static float GetTime() { return glfwGetTime(); }
	
	static bool Update();
	static Color GetClearColor();
	static void SetClearColor(const Color color);
	static void Cleanup();

	static int counter;

private:
	static Window* sWindow;
	static Camera* sCamera;
	//static Shader* sShader;
	static std::vector<Shader*> sShaders;
	static std::vector<uint> sTextures;
	static std::vector<RenderObject> sRenderObjects;
	static std::vector<BaseLight*> sLights;
	static std::vector<Material> sMaterials;
	static bool useWireframe;
	

	static uint LoadObject();
	//static bool LoadBuffers(const Geometry& geometry);
	static void UpdateFlyCamControls();
	static Geometry BuildQuad();
};