#include "GLFramework.h"
#include "..\include\GLFramework.h"

Window* GLFramework::sWindow = new Window();
int GLFramework::counter = 0;
Camera* GLFramework::sCamera = new Camera();
Shader* GLFramework::sShader = new Shader();
//std::vector<RenderObject*> GLFramework::sRenderObjects = std::vector<RenderObject*>();
RenderObject* GLFramework::grid = nullptr;

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

	LoadObject();

	return true;
}

bool GLFramework::SetShader(const char * vertexPath, const char * fragmentPath)
{
	return sShader->LoadShader(vertexPath, fragmentPath);
}

bool GLFramework::SetCameraView(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up)
{
	return sCamera->SetView(position, target, up);
}

bool GLFramework::SetCameraProjection(const float fov, const float aspectRatio, const float a_near, const float a_far)
{
	return sCamera->StartupPerspective(fov, aspectRatio, a_near, a_far);
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//sShader->SetUniform("ProjectionView", Shader::MAT4, &sCamera->GetViewProjection());
	uint projectionViewUniform = glGetUniformLocation(sShader->GetProgram(), "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(sCamera->GetProjection() * sCamera->GetView()));
	glBindVertexArray(grid->vao);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid->ibo);
	//glBindBuffer(GL_ARRAY_BUFFER, grid->vbo);
	glDrawElements(GL_TRIANGLES, grid->size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//for each (RenderObject* object in sRenderObjects)
	//{
	//	//glBindVertexArray(object->vao);
	//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->ibo);
	//	//glBindBuffer(GL_ARRAY_BUFFER, object->vbo);
	//	glDrawElements(GL_TRIANGLES, object->size, GL_UNSIGNED_INT, 0);
	//	//glBindVertexArray(0);
	//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//}



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
	if (nullptr != grid)
	{
		delete grid;
	}

	//for (int i = 0; i < sRenderObjects.size(); i++)
	//{
	//	delete sRenderObjects[i];
	//}
	//sRenderObjects.clear();


}

uint GLFramework::LoadObject()
{
	int rows = 25, cols = 25;
	uint verticesSize = rows * cols;
	Vertex* vertices = new Vertex[verticesSize];
	for (uint r = 0; r < rows; r++)
	{
		for (uint c = 0; c < cols; c++)
		{
			vertices[r * cols + c].position = vec4((float)c, 0, (float)r, 1);

			vec3 color = vec3(sinf((c / (float)(cols - 1))*(r / (float)(rows - 1))));

			vertices[r*cols + c].color = vec4(color, 1);
		}
	}



	uint indecesCount = (rows - 1) * (cols - 1) * 6;
	uint* indeces = new uint[indecesCount];

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
	}


	Geometry* geometry = new Geometry();
	geometry->indeces = indeces;
	geometry->indecesSize = indecesCount;
	geometry->vertices = vertices;
	geometry->verticesSize = verticesSize;
	grid = new RenderObject();
	grid->LoadBuffers(*geometry);


	//sRenderObjects.push_back(grid);



	delete[] vertices;
	delete[] indeces;
	delete geometry;

	return 0;
}
