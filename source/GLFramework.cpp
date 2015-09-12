#include "..\include\GLFramework.h"

Window* GLFramework::sWindow = new Window();
FlyCamera* GLFramework::sCamera;

bool GLFramework::Startup(const int height, const int width, const char * title, const vec4 clearColor)
{
	if (!glfwInit())
	{
		return false;
	}
	sWindow->height = height;
	sWindow->width = width;
	sWindow->title = title;

	sWindow->handle = glfwCreateWindow(height, width, title, nullptr, nullptr);


	if (nullptr == sWindow->handle)
	{
		glfwTerminate();
		return false;
	}



	glfwMakeContextCurrent(sWindow->handle);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(sWindow->handle);
		glfwTerminate();
		return false;
	}

	SetClearColor(clearColor);
	glEnable(GL_DEPTH_TEST);

	return true;
}

vec4 GLFramework::GetClearColor()
{
	vec4 result;
	if (nullptr != sWindow)
	{
		result = sWindow->clearColor;
	}
	return result;
}

void GLFramework::SetClearColor(vec4 color)
{
	if (nullptr != sWindow)
	{
		sWindow->clearColor = color;
		glClearColor(sWindow->clearColor.r, sWindow->clearColor.g, sWindow->clearColor.b, sWindow->clearColor.a);
	}
}

void GLFramework::Cleanup()
{
	delete sWindow;
}
