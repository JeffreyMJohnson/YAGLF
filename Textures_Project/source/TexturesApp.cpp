#include "TexturesApp.h"

typedef GLFramework glf;

bool TexturesApp::StartUp()
{
	glf::Startup(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, CLEAR_COLOR);
	glf::SetShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	mTexture = glf::LoadTexture("../resources/textures/rock_diffuse.tga");
	mNormalMap = glf::LoadTexture("../resources/textures/rock_normal.tga");

	glf::SetTexture(Texture_Unit::ONE, mNormalMap);
	glf::SetShaderUniform("normal", Shader::INT1, &mNormalMap);
	glf::SetTexture(Texture_Unit::ZERO, mTexture);
	glf::SetShaderUniform("diffuse", Shader::INT1, &mTexture);
	glf::SetCameraProjection(CAMERA_FOV, WINDOW_WIDTH / (float)WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	glf::SetCameraView(CAMERA_FROM, CAMERA_TO, CAMERA_UP);
	glf::LoadModel(BuildQuad());


	//glf::LoadModel("../resources/models/fbx/soulspear.fbx");
	return true;
}

void TexturesApp::ShutDown()
{
	glf::Cleanup();
}

bool TexturesApp::Update()
{
	vec3 light(sin(glf::GetTime()), 1, cos(glf::GetTime()));
	glf::SetShaderUniform("lightDirection", Shader::VEC3, &light);
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
	v1.normal = vec4(0, 1, 0, 0);
	v1.tangent = vec4(1, 0, 0, 0);
	v1.UV = vec2(0, 1);
	quad.vertices.push_back(v1);
	Vertex v2;
	v2.position = vec4(5, 0, 5, 1);
	v2.normal = vec4(0, 1, 0, 0);
	v2.tangent = vec4(1, 0, 0, 0);
	v2.UV = vec2(1, 1);
	quad.vertices.push_back(v2);
	Vertex v3;
	v3.position = vec4(5, 0, -5, 1);
	v3.normal = vec4(0, 1, 0, 0);
	v3.tangent = vec4(1, 0, 0, 0);
	v3.UV = vec2(1, 0);
	quad.vertices.push_back(v3);
	Vertex v4;
	v4.position = vec4(-5, 0, -5, 1);
	v4.normal = vec4(0, 1, 0, 0);
	v4.tangent = vec4(1, 0, 0, 0);
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