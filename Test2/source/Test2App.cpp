#include "Test2App.h"


typedef Dryv3r dvr;

bool Test2App::StartUp()
{
	if (!dvr::Init(1024, 768, "Dryv3r Test")) return false;
	testObject = dvr::GetCube();
	test2 = dvr::GetQuad();
	test2.transform.GetTransform() = glm::rotate(90.0f, vec3(1,0,0)) * glm::scale(vec3(10, 10, 1));
	crateTexture = dvr::LoadTexture("../resources/textures/crate.png");
	testObject.diffuse = crateTexture;
	test2.diffuse = crateTexture;
	mesh = dvr::loadMesh("../resources/models/fbx/soulspear.fbx");
	spearDiffuse = dvr::LoadTexture("../resources/textures/soulspear/soulspear_diffuse.tga");
	testObject.mesh = mesh;
	testObject.diffuse = spearDiffuse;
	return true;
}

void Test2App::ShutDown()
{
	dvr::Cleanup();
}

bool Test2App::Update()
{



	if (Keyboard::IsKeyPressed(Keyboard::KEY_KP_7))
	{
		dvr::SetDirectionalLightDirection(vec3(0, 0, 1));
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_KP_8))
	{
		dvr::useDirectionalLight = !dvr::useDirectionalLight;
	}

	testObject.transform.GetTransform() *= glm::rotate(dvr::GetDeltaTime(), vec3(0, 1, 0));
	//test2.transform.GetTransform() *= glm::rotate(dvr::GetDeltaTime(), vec3(1,0, 0));
	
	return dvr::Update();
}

void Test2App::Draw()
{
	dvr::DrawGameObject(testObject);
	dvr::DrawGameObject(test2);
	dvr::Draw();
}
