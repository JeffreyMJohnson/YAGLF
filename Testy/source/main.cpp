#include "GLFramework.h"
#include <iostream>

typedef GLFramework glf;
typedef glm::vec3 vec3;


void main()
{
	using namespace std;
	//camera = new FlyCamera();
	//camera->SetView(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	//camera->SetPerspective(glm::pi<float>() * .25f, (float)1280 / 720, .1f, 1000.0f);

	bool start = glf::Startup(1280, 720, "foo", Color(1, 1, 1, 1));
	if (!start)
	{
		std::cout << "Failed startup.\n";
	}
	else
	{
		std::cout << "Startup succeded.\n";
	}

	bool shader = glf::SetShader("../Testy/source/Simple_Vertex_shader.glsl", "../Testy/source/Simple_Fragment_Shader.glsl");
	if (!shader)
	{
		std::cout << "Failed loading shaders.\n";
	}
	else
	{
		std::cout << "Loading shaders succeded.\n";
	}

	bool camera = glf::SetCamera(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	if (!camera)
	{
		std::cout << "Failed loading camera.\n";
	}
	else
	{
		std::cout << "Loading camera succeded.\n";
	}

	while (glf::Update())
	{
		
	}
	//system("pause");
	glf::Cleanup();
}