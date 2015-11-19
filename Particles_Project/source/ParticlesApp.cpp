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

	mEmitter = new ParticleEmitter();
	mEmitter->Init(
		1000, //max particles
		100, //emit rate
		.1f, 2.0f, //min-max life time
		.1, 5, //min-max velocity
		1, .1f, //start-end size
		glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1)); // start-endcolor


	return true;
}

void ParticlesApp::ShutDown()
{
	glf::Cleanup();
}

bool ParticlesApp::Update()
{
	UpdateFlyCamControls();

	mEmitter->Update(glf::GetDeltaTime(), mMainCamera.GetWorldTransform());

	return glf::Update() && !(glf::WindowShouldClose());

}

void ParticlesApp::Draw()
{
	glf::UseShader(mShaderProgram);
	glf::SetShaderUniform(mShaderProgram, "View", Shader::MAT4, glm::value_ptr(mMainCamera.GetView()));
	glf::SetShaderUniform(mShaderProgram, "Projection", Shader::MAT4, glm::value_ptr(mMainCamera.GetProjection()));
	mEmitter->Draw();
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
