#pragma once
#include "GameApp.h"
#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include <assert.h>

struct GameObject
{
	GLuint vao = 0;
	uint triCount = 0;
	GLuint diffuseTexture = 0;
};

class ShadowsApp : public GameApp
{
public:

	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	const char* WINDOW_TITLE = "Shadows App";
	const std::string SHADER_PATH = "../Shadows_Project/source/shaders/";
	const Color CLEAR_COLOR = Color(.25f, .25f, .25f, 1);;
	const float CAMERA_FOV = glm::pi<float>() *.25f;
	const float CAMERA_NEAR = .1f;
	const float CAMERA_FAR = 1000.0f;
	const vec3 CAMERA_FROM = vec3(0,2,5);
	const vec3 CAMERA_TO = vec3(0,2,0);
	const vec3 CAMERA_UP = vec3(0, 1, 0);


	bool StartUp();
	void ShutDown();
	bool Update();
	void Draw();
private:
	//buffers
	uint mGPassFBO;
	uint mGPassDepth;
	uint mLightFBO;

	//textures
	uint mAlbedoTexture;
	uint mPositionTexture;
	uint mNormalTexture;
	uint mLightTexture;

	//uint mQuadVAO, mVBO, mIBO;
	GameObject mQuad;

	Window window;
	//Shader mGBufferShader;
	uint mGBufferShader;
	//Shader mCompositeShader;
	uint mCompositeShader;
	//Shader mDirectionalLightShader;
	uint mDirectionalLightShader;
	Camera camera;

	std::vector<GameObject> mSceneGeometry;
	FBXFile mFBX;

	void LoadModel(std::string path);
	void LoadModel(Geometry& geometry);
	void drawDirectionalLight(const glm::vec3& direction, const glm::vec3& diffuse);
};
