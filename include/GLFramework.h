#pragma once
#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "glm\vec4.hpp"
#include "camera\Camera.h"
#include "Shader.h"
#include "RenderObject.h"

#include <iostream>
#include <map>
#include <vector>

using glm::vec4;
using glm::vec3;

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
};


struct Window
{
	int height = 0;
	int width = 0;
	char* title = "";
	GLFWwindow* handle = nullptr;
	Color clearColor;
};


class GLFramework
{
public:
	struct Vertex
	{
		vec4 position;
		vec4 color;
	};

	static bool Startup(int height, int width, char* title, Color clearColor);
	static bool SetShader(const char* vertexPath, const char* fragmentPath);
	static bool SetCamera(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up);
	static void SlideCamera(const float hDistance, const float vDistance);
	static void MoveCamera(const float distance);
	static bool Update();
	static Color GetClearColor();
	static void SetClearColor(Color color);
	static void Cleanup();

	static int counter;

private:
	static Window* sWindow;
	static Camera* sCamera;
	static Shader* sShader;
	static std::map<uint, RenderObject*> sRenderObjects;

	static uint LoadObject();
};