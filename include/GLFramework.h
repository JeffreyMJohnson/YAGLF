#pragma once
#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "glm\vec4.hpp"
#include "camera\FlyCamera.h"

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
	char* title = "";
	GLFWwindow* handle = nullptr;
	Color clearColor;
	bool isClosing = false;
};


class GLFramework
{
public:
	static bool Startup(int height, int width, char* title, Color clearColor);
	static bool Update();
	static Color GetClearColor();
	static void SetClearColor(Color color);
	static void Cleanup();

private:
	static Window* sWindow;
	static Camera* sCamera;
};