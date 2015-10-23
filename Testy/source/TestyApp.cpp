#include "TestyApp.h"

typedef unsigned int uint;
typedef GLFramework glf;

bool TestyApp::StartUp()
{
	glf::Startup(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, CLEAR_COLOR);
	glf::SetShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	glf::SetCameraProjection(CAMERA_FOV, WINDOW_WIDTH / (float)WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	glf::SetCameraView(CAMERA_FROM, CAMERA_TO, CAMERA_UP);

	return true;
}

void TestyApp::ShutDown()
{
	glf::Cleanup();
}

bool TestyApp::Update()
{
	return glf::Update();
}

void TestyApp::Draw()
{
}
