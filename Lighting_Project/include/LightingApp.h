#pragma once
#include "GameApp.h"

class LightingApp : public GameApp
{
public:

	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	const char* WINDOW_TITLE = "Textures App";
	const char* VERTEX_SHADER_PATH = "../Textures_Project/source/shaders/Texture_Vert_shader.glsl";
	const char* FRAGMENT_SHADER_PATH = "../Textures_Project/source/shaders/Texture_Frag_Shader.glsl";
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

};
