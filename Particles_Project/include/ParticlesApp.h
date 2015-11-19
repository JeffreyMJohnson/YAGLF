#pragma once
#include "GameApp.h"
#include "Camera.h"
#include "ParticleEmitter.h"



struct GameObject
{
	glm::mat4 transform;
	uint geometry = 0;
};

class ParticlesApp : public GameApp
{
	uint mShaderProgram = 0;
	Geometry mTriangle;
	GameObject mGameObj;
	Camera mMainCamera;
	ParticleEmitter* mEmitter;

	void UpdateFlyCamControls();
	

public:
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	const char* WINDOW_TITLE = "Textures App";
	const char* VERTEX_SHADER_PATH = "./source/shaders/vertex.glsl";
	const char* FRAGMENT_SHADER_PATH = "./source/shaders/fragment.glsl";
	const vec4 CLEAR_COLOR = vec4(.75f, .75f, .75f, 1);
	const float CAMERA_FOV = glm::pi<float>() * .25f;
	const float CAMERA_NEAR = .1f;
	const float CAMERA_FAR = 1000.0f;
	const vec3 CAMERA_FROM = vec3(0, 10, 10);
	const vec3 CAMERA_TO = vec3(0);
	const vec3 CAMERA_UP = vec3(0, 1, 0);

	bool StartUp();
	void ShutDown();
	bool Update();
	void Draw();
};