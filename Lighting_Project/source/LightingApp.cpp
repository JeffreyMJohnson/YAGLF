#include "LightingApp.h"

typedef GLFramework glf;

bool LightingApp::StartUp()
{
	glf::Startup(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, CLEAR_COLOR);
	glf::SetShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	glf::SetCameraProjection(CAMERA_FOV, WINDOW_WIDTH / (float)WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	glf::SetCameraView(CAMERA_FROM, CAMERA_TO, CAMERA_UP);
	glf::LoadModel("../resources/models/fbx/bunny.fbx");

	light = glf::SetDirectionalLight(RED, vec3(0, 0, 0));

	return true;
}

void LightingApp::ShutDown()
{

}

bool LightingApp::Update()
{
	count += .01f;
	glf::SetLightDirection(light, vec3(0, count, 0));
	return glf::Update();
}

void LightingApp::Draw()
{
}
