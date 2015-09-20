#include "TexturesApp.h"

typedef GLFramework glf;

bool TexturesApp::StartUp()
{
	glf::Startup(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, CLEAR_COLOR);
	glf::SetShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	textures.push_back(glf::LoadTexture("../resources/textures/soulspear/soulspear_diffuse.tga"));
	glf::SetTexture(Texture_Unit::ZERO, textures[0]);
	glf::SetShaderUniform("diffuse", Shader::INT1, &textures[0]);
	glf::SetCameraProjection(CAMERA_FOV, WINDOW_WIDTH / (float)WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	glf::SetCameraView(CAMERA_FROM, CAMERA_TO, CAMERA_UP);
	//glf::LoadModel(BuildQuad());
	glf::LoadModel("../resources/models/fbx/soulspear.fbx");
	return true;
}

void TexturesApp::ShutDown()
{
	glf::Cleanup();
}

bool TexturesApp::Update()
{
	return glf::Update();
}

void TexturesApp::Draw()
{

}

Geometry TexturesApp::BuildQuad()
{
	Geometry quad;
	Vertex v1;
	v1.position = vec4(-5, 0, 5, 1);
	v1.UV = vec2(0, 1);
	quad.vertices.push_back(v1);
	Vertex v2;
	v2.position = vec4(5, 0, 5, 1);
	v2.UV = vec2(1, 1);
	quad.vertices.push_back(v2);
	Vertex v3;
	v3.position = vec4(5, 0, -5, 1);
	v3.UV = vec2(1, 0);
	quad.vertices.push_back(v3);
	Vertex v4;
	v4.position = vec4(-5, 0, -5, 1);
	v4.UV = vec2(0, 0);
	quad.vertices.push_back(v4);

	quad.indices.push_back(0);
	quad.indices.push_back(1);
	quad.indices.push_back(2);
	quad.indices.push_back(0);
	quad.indices.push_back(2);
	quad.indices.push_back(3);

	return quad;
}