#include "ParticlesApp.h"

typedef unsigned int uint;
typedef GLFramework glf;


bool ParticlesApp::StartUp()
{
	glf::Startup(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, CLEAR_COLOR);
	mShaderProgram = glf::LoadShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	//init camera
	mMainCamera.SetPerspectiveProjection(CAMERA_FOV, WINDOW_WIDTH / (float)WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	mMainCamera.SetView(CAMERA_FROM, CAMERA_TO, CAMERA_UP);

	
	return true;
}

void ParticlesApp::ShutDown()
{
	glf::Cleanup();
}

bool ParticlesApp::Update()
{
	UpdateFlyCamControls();
	glf::UseShader(mShaderProgram);
	
	
	return glf::Update() && !(glf::WindowShouldClose());
	
}

void ParticlesApp::Draw()
{

}

void ParticlesApp::UpdateFlyCamControls()
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
