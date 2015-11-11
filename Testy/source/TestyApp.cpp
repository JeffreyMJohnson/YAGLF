#include "TestyApp.h"

typedef unsigned int uint;
typedef GLFramework glf;


bool TestyApp::StartUp()
{
	glf::Startup(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, CLEAR_COLOR);
	mShaderProgram = glf::LoadShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	//init camera
	mMainCamera.SetPerspectiveProjection(CAMERA_FOV, WINDOW_WIDTH / (float)WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	mMainCamera.SetView(CAMERA_FROM, CAMERA_TO, CAMERA_UP);

	std::vector<Vertex> vertices;
	vertices.resize(3);
	Vertex v0;
	v0.position = vec4(.25f, -.25f, .5f, 1.0f);
	vertices[0] = v0;
	Vertex v1;
	v1.position = vec4(-.25f, -.25f, .5f, 1.0f);
	vertices[1] = v1;
	Vertex v2;
	v2.position = vec4(.25f, .25f, .5f, 1.0f);
	vertices[2] = v2;
	std::vector<uint> indices{ 0,1,2 };
	mTriangle.vertices = vertices;
	mTriangle.indices = indices;
	//mGameObj.geometry = glf::LoadModel(mTriangle);

	//set obj as premade model cube
	mGameObj.geometry = glf::GeometryModel::CUBE;
	return true;
}

void TestyApp::ShutDown()
{
	glf::Cleanup();
}

bool TestyApp::Update()
{
	UpdateFlyCamControls();
	glf::UseShader(mShaderProgram);
	
	mGameObj.transform = glm::rotate(mGameObj.transform, glf::GetDeltaTime(), glm::vec3(0, 1, 0));
	
	glf::SetShaderUniform(mShaderProgram, "Model", Shader::UniformType::MAT4, glm::value_ptr(mGameObj.transform));
	glf::SetShaderUniform(mShaderProgram, "View", Shader::UniformType::MAT4, glm::value_ptr(mMainCamera.GetView()));
	glf::SetShaderUniform(mShaderProgram, "Projection", Shader::UniformType::MAT4, glm::value_ptr(mMainCamera.GetProjection()));
	
	return glf::Update() && !(glf::WindowShouldClose());
	
}

void TestyApp::Draw()
{
	glf::DrawModel(mGameObj.geometry);
}

void TestyApp::UpdateFlyCamControls()
{
	if (Keyboard::IsKeyPressed(Keyboard::KEY_W) || Keyboard::IsKeyRepeat(Keyboard::KEY_W))
	{
		mMainCamera.Move(1);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_X) || Keyboard::IsKeyRepeat(Keyboard::KEY_X))
	{
		mMainCamera.Move(-1);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_A) || Keyboard::IsKeyRepeat(Keyboard::KEY_A))
	{
		mMainCamera.Slide(-1, 0);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_D) || Keyboard::IsKeyRepeat(Keyboard::KEY_D))
	{
		mMainCamera.Slide(1, 0);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_E) || Keyboard::IsKeyRepeat(Keyboard::KEY_E))
	{
		mMainCamera.Slide(0, 1);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_C) || Keyboard::IsKeyRepeat(Keyboard::KEY_C))
	{
		mMainCamera.Slide(0, -1);
	}
}
