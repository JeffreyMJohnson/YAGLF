#include "LightingApp.h"

typedef GLFramework glf;

bool LightingApp::StartUp()
{
	glf::Startup(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, CLEAR_COLOR);
	glf::SetShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	glf::SetCameraProjection(CAMERA_FOV, WINDOW_WIDTH / (float)WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	glf::SetCameraView(CAMERA_FROM, CAMERA_TO, CAMERA_UP);
	glf::LoadModel("../resources/models/fbx/bunny.fbx");

	//uint directionalLight = glf::CreateLight(glm::vec3(0, 1, 0), WHITE, RED, WHITE);


	//directional light properties
	glm::vec3 lightDirection = glm::vec3(0,1, 0);
	glm::vec3 lightDiffuse = vec3(.99f, .72f, .07f);
	glm::vec3 lightAmbient = vec3(.25f, .25f, .25f);
	vec3 lightSpecular = vec3(1, 1, 1);

	//material properties
	float specularPower = 128;
	vec3 matDiffuseColor = vec3(1, 1, 1);
	vec3 matAmbientColor = vec3(0);
	vec3 matSpecularColor = vec3(0, 1, 0);
	
	glf::SetShaderUniform("lightDirection", Shader::VEC3, &lightDirection);
	glf::SetShaderUniform("lightAmbient", Shader::VEC3, &lightAmbient);
	glf::SetShaderUniform("lightDiffuse", Shader::VEC3, &lightDiffuse);
	glf::SetShaderUniform("lightSpecular", Shader::VEC3, &lightSpecular);
	glf::SetShaderUniform("cameraPosition", Shader::VEC3, &glf::GetCameraPosition());


	//glf::SetShaderUniform("specularPower", Shader::FLO1, &specularPower);
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
