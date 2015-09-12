#pragma once
#include "gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "glm\vec4.hpp"
#include "FlyCamera.h"
#include "Transform.h"

#include <iostream>



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
};


struct Window
{
	int height = 0;
	int width = 0;
	const char* title = "";
	GLFWwindow* handle = nullptr;
	vec4 clearColor;
};


class GLFramework
{
public:
	static bool Startup(const int height, const int width, const char* title, const vec4 clearColor);
	static vec4 GetClearColor();
	static void SetClearColor(vec4 color);
	static void Cleanup();

private:
	static Window* sWindow;
	static FlyCamera* sCamera;
};