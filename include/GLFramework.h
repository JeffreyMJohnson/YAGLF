#pragma once
#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "glm\vec4.hpp"
#include "camera\Camera.h"
#include "Shader.h"
#include "RenderObject.h"
#include "tiny_obj_loader\tiny_obj_loader.h"
#include "fbx_loader\FBXFile.h"
#include "stb\stb_image.h"

#include <iostream>
#include <map>
#include <vector>

using glm::vec2;
using glm::vec3;
using glm::vec4;


struct Color
{
	float r = 0, g = 0, b = 0, a = 0;
	Color() {};
	Color(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};


struct Window
{
	int height = 0;
	int width = 0;
	const char* title;
	GLFWwindow* handle = nullptr;
	Color clearColor;
};

enum Texture_Unit
{
	ZERO = GL_TEXTURE0,
	ONE = GL_TEXTURE1,
	TWO = GL_TEXTURE2,
	THREE = GL_TEXTURE3,
	FOUR = GL_TEXTURE4,
	FIVE = GL_TEXTURE5,
	SIX = GL_TEXTURE6,
	SEVEN = GL_TEXTURE7,
	EIGHT = GL_TEXTURE8,
	NINE = GL_TEXTURE9,
	TEN = GL_TEXTURE10
};

class GLFramework
{
public:


	static bool Startup(const int width, const int height, const char* title, const Color clearColor);
	static bool SetShader(const char* vertexPath, const char* fragmentPath);
	static void SetShaderUniform(const char* name, const Shader::UniformType type, const void* value);
	static uint LoadTexture(const char * path);
	static void SetTexture(Texture_Unit unit, uint texture);

	static void SetWireframe(bool value);
	static bool SetCameraView(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up);
	static bool SetCameraProjection(const float fov, const float aspectRatio, const float a_near, const float a_far);
	static vec3 GetCameraPosition() { return sCamera->GetPosition(); }
	static void SlideCamera(const float hDistance, const float vDistance);
	static void MoveCamera(const float distance);

	static bool LoadModel(const char* path);
	static bool LoadModel(Geometry& geometry);

	static bool Update();
	static Color GetClearColor();
	static void SetClearColor(const Color color);
	static void Cleanup();

	static int counter;

private:
	static Window* sWindow;
	static Camera* sCamera;
	static Shader* sShader;
	static std::vector<uint> sTextures;
	static RenderObject* sRenderObject;
	static bool useWireframe;

	static uint LoadObject();
	static bool LoadBuffers(const Geometry& geometry);
};