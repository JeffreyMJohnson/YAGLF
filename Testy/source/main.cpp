#include "GLFramework.h"
#include "camera\FlyCamera.h"
#include <iostream>

typedef GLFramework glf;
typedef glm::vec3 vec3;

FlyCamera* camera = nullptr;

void main()
{
	using namespace std;
	camera = new FlyCamera();
	camera->SetView(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	camera->SetPerspective(glm::pi<float>() * .25f, (float)1280 / 720, .1f, 1000.0f);

	glf::Startup(1280, 720, "foo", Color(1, 1, 1, 1));
	while (glf::Update())
	{
		
	}
	//system("pause");
	glf::Cleanup();
	if (nullptr != camera)
	{
		delete camera;
	}
}