#include "GLFramework.h"
#include "..\include\GLFramework.h"

Window* GLFramework::sWindow = new Window();
int GLFramework::counter = 0;
Camera* GLFramework::sCamera = new Camera();
Shader* GLFramework::sShader = new Shader();
std::map<uint, RenderObject*> GLFramework::sRenderObjects;

bool GLFramework::Startup(int height, int width, char * title, Color clearColor)
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return true;
}

bool GLFramework::SetShader(const char * vertexPath, const char * fragmentPath)
{
	return sShader->LoadShader(vertexPath, fragmentPath);
}

bool GLFramework::SetCamera(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up)
{
	return sCamera->StartupPerspective(position, target, up);
}

void GLFramework::SlideCamera(const float hDistance, const float vDistance)
{
	sCamera->Slide(hDistance, vDistance);
}

void GLFramework::MoveCamera(const float distance)
{
	sCamera->Move(distance);
}

bool GLFramework::Update()
{
	if (glfwWindowShouldClose(sWindow->handle) || glfwGetKey(sWindow->handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(sWindow->handle);
	glfwPollEvents();
	return true;
}

Color GLFramework::GetClearColor()
{
	Color result;
	if (nullptr != sWindow)
	{
		result = sWindow->clearColor;
	}
	return result;
}

void GLFramework::SetClearColor(Color color)
{
	if (nullptr != sWindow)
	{
		sWindow->clearColor = color;
		glClearColor(sWindow->clearColor.r, sWindow->clearColor.g, sWindow->clearColor.b, sWindow->clearColor.a);
	}
}

void GLFramework::Cleanup()
{
	if (nullptr != sWindow)
	{
		delete sWindow;
	}
	if (nullptr != sCamera)
	{
		delete sCamera;
	}
	if (nullptr != sShader)
	{
		delete sShader;
	}

	for (int i = 0; i < sRenderObjects.size(); i++)
	{
		delete sRenderObjects[i];
	}
	sRenderObjects.clear();
}

uint GLFramework::LoadObject()
{
	int rows = 1, cols = 1;
	
	Vertex* vertices = new Vertex[rows * cols];
	for (uint r = 0; r < rows; r++)
	{
		for (uint c = 0; c < cols; c++)
		{
			vertices[r * cols + c].position = vec4((float)c, 0, (float)r, 1);

			vec3 color = vec3(sinf((c / (float)(cols - 1))*(r / (float)(rows - 1))));

			vertices[r*cols + c].color = vec4(color, 1);
		}
	}
	/*
	uint* indeces = new uint[(rows - 1) * (cols - 1) * 6];
	uint index = 0;
	for (uint r = 0; r < (rows - 1); r++)
	{
		for (uint c = 0; c < (cols - 1); c++)
		{
			//triangle 1
			indeces[index++] = r*cols + c;
			indeces[index++] = (r + 1)*cols + c;
			indeces[index++] = (r + 1)*cols + (c + 1);

			//triangle 2
			indeces[index++] = r*cols + c;
			indeces[index++] = (r + 1)*cols + (c + 1);
			indeces[index++] = r*cols + (c + 1);
		}
	}*/


}
