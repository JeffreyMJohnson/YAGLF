#pragma once
#include "GameApp.h"
#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "../../include/gizmos/Gizmos.h"

class RenderTargetsApp : public GameApp
{
public:
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	const char* WINDOW_TITLE = "Render Targetgs App";
	const char* VERTEX_SHADER_PATH = "../Render_Targets_Project/source/shaders/Vert.glsl";
	const char* FRAGMENT_SHADER_PATH = "../Render_Targets_Project/source/shaders/Frag.glsl";
	const Color CLEAR_COLOR = Color(.25f, .25f, .25f, 1);
	const float CAMERA_FOV = glm::pi<float>() * .25f;
	const float CAMERA_NEAR = .1f;
	const float CAMERA_FAR = 1000.0f;
	const vec3 CAMERA_FROM = vec3(10, 10, 10);
	const vec3 CAMERA_TO = vec3(0);
	const vec3 CAMERA_UP = vec3(0, 1, 0);

	bool StartUp();
	void ShutDown();
	bool Update();
	void Draw();

private:
	uint mFBO;
	uint mFBOTexture;
	uint mDepthBuffer;

	uint VAO, VBO, IBO;


	Window window;
	Shader shader;
	Camera camera;

};

