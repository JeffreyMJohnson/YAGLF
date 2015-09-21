#include "LightingApp.h"

typedef GLFramework glf;

bool LightingApp::StartUp()
{
	glf::Startup(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, CLEAR_COLOR);
	glf::SetShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	glf::SetCameraProjection(CAMERA_FOV, WINDOW_WIDTH / (float)WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	glf::SetCameraView(CAMERA_FROM, CAMERA_TO, CAMERA_UP);
	glf::LoadModel("../resources/models/fbx/bunny.fbx");

	glm::vec3 lightDirection = glm::vec3(0,1, 0);
	glf::SetShaderUniform("uLightDirection", Shader::VEC3, &lightDirection);
	return true;
}

void LightingApp::ShutDown()
{

}

bool LightingApp::Update()
{
	return glf::Update();
}

void LightingApp::Draw()
{
}
