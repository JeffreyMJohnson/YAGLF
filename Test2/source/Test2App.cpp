#include "Test2App.h"


typedef Dryv3r dvr;

bool Test2App::StartUp()
{
	if (!dvr::Init(1024, 768, "Dryv3r Test")) return false;
	testObject = dvr::GetCube();
	test2 = dvr::GetQuad();
	test2.transform.GetTransform() = glm::translate(vec3(-2, 0, 2));
	crateTexture = dvr::LoadTexture("../resources/textures/crate.png");
	testObject.diffuse = crateTexture;
	//test2.diffuse = crateTexture;
	return true;
}

void Test2App::ShutDown()
{
	dvr::Cleanup();
}

bool Test2App::Update()
{
	dvr::DrawGameObject(testObject);
	dvr::DrawGameObject(test2);
	return dvr::Update();
}

void Test2App::Draw()
{
	dvr::Draw();
}
