#include "..\include\RenderTargetsApp.h"

typedef unsigned int uint;
typedef GLFramework glf;

bool RenderTargetsApp::StartUp()
{
	//startup
	if (!glfwInit())
	{
		return false;
	}

	window.width = WINDOW_WIDTH;
	window.height = WINDOW_HEIGHT;
	window.title = WINDOW_TITLE;

	window.handle = glfwCreateWindow(window.width, window.height, window.title, nullptr, nullptr);

	if (nullptr == window.handle)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window.handle);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window.handle);
		glfwTerminate();
		return false;
	}

	window.clearColor = CLEAR_COLOR;
	glClearColor(window.clearColor.r, window.clearColor.g, window.clearColor.b, window.clearColor.a);



	if (!shader.LoadShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH))
	{
		return false;
	}

	camera.StartupPerspective(CAMERA_FOV, (float)WINDOW_WIDTH / WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	camera.SetView(CAMERA_FROM, CAMERA_TO, CAMERA_UP);




	return true;
}

void RenderTargetsApp::ShutDown()
{

}

bool RenderTargetsApp::Update()
{
	if (glfwWindowShouldClose(window.handle) || glfwGetKey(window.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		return false;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);



	shader.SetUniform("ProjectionView", Shader::MAT4, glm::value_ptr(camera.GetViewProjection()));

	glfwSwapBuffers(window.handle);
	glfwPollEvents();
	return true;
}

void RenderTargetsApp::Draw()
{
}
