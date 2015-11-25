#pragma once
#define GLM_SWIZZLE
//#define STB_IMAGE_IMPLEMENTATION

#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "fbx_loader\FBXFile.h"

#include "stb\stb_image.h"

#include "Geometry.h"
#include "Shader.h"
#include "Camera.h"

#include <iostream>
#include <string>
#include <assert.h>
#include <vector>
#include <map>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

struct Window
{
	int height = 0;
	int width = 0;
	const char* title;
	GLFWwindow* handle = nullptr;
};

struct Mesh
{
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ibo = 0;
	GLuint count;
};

class Transform
{
public:
	vec3 GetPosition() { return mPosition; }
	void SetPosition(vec3 position)
	{
		mPosition = position;
		mTransform = glm::translate(mTransform, position);
	}
	mat4 GetTransform() { return mTransform; }

private:
	mat4 mTransform;
	vec3 mPosition;
};

struct Texture
{
	GLuint name;
	GLsizei width, height;

};

class GameObject
{
public:
	Transform transform;
	Mesh mesh;
	Texture diffuse;
	
};



struct FrameBuffer
{
	GLuint name;
	Shader shader;
};

class Dryv3r
{
public:
	/*
	Initialize the framework and create a window with given size and title
	*/
	static bool Init(const int width, const int height, const char* title);

	/*
	Call once per frame
	*/
	static bool Update();

	static void Cleanup();

	/*
	Call after updating everything in frame and ready to send to screen.
	*/
	static void Draw();

	/*
	Call when want to draw the given object to the screen this frame.
	*/
	static void DrawGameObject(GameObject& gameObject);
	
	/*
	returns reference to 3D cube game object
	*/
	static GameObject& GetCube() { return mCube; }
	
	/*
	returns ref to simple 2d triangle in screenspace for early testing
	*/
	static GameObject& GetTestTri() { return mTestTri; }

	/*
	Loads a texture from givin file with given name string for future reference.
	*/
	static Texture* LoadTexture(const char* filePath);
	//applytexture
	//destroytexture

private:
	
	static Window mWindow;
	static GameObject mCube;
	static GameObject mTestTri;
	static std::vector<GameObject*> mObjectsToDraw;
	static Shader mShader;
	static Camera mCamera;
	static Texture mTexture;
	static std::map<std::string, Texture> mTextureList;

	static Mesh LoadMesh(Geometry& geometry);
	static void DrawMesh(Mesh& mesh);
	
	/*
	returns texture object of given size and format
	*/
	static Texture MakeTexture(GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);

	/*
	Load pixel data into texture
	*/
	static void LoadTextureData(Texture& texture, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);

	/*
	Main camera default settings:
	perspective
	fov = 90
	aspect ratio = current window width/height
	near = .1
	far = 1000
	View - 
	position = vec3(5)
	look at = vec3(0)
	up = vec3(0,1,0)
	*/
	static void InitCamera();


};
