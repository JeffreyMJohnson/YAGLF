#include "Test2App.h"

typedef Dryv3r dvr;

bool Test2App::StartUp()
{
	if (!dvr::Init(1024, 768, "Dryv3r Test")) return false;
	testTri = dvr::GetTestTri();
	return true;
}

void Test2App::ShutDown()
{
	dvr::Cleanup();
}

bool Test2App::Update()
{
	dvr::DrawGameObject(testTri);
	return dvr::Update();
}

void Test2App::Draw()
{
	dvr::Draw();
}
