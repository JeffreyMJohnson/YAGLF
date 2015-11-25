#include "Dryv3r.h"

Window Dryv3r::mWindow;
GameObject Dryv3r::mCube;
GameObject Dryv3r::mTestTri;
std::vector<GameObject*> Dryv3r::mObjectsToDraw;
Shader Dryv3r::mShader;
Camera Dryv3r::mCamera;
Texture Dryv3r::mTexture;


bool Dryv3r::Init(const int width, const int height, const char * title)
{
	if (!glfwInit())
	{
//#ifdef _DEBUG
		assert(false && "Error initializing GLFW.");
//#endif
		return false;
	}
	mWindow.height = height;
	mWindow.width = width;
	mWindow.title = title;
	mWindow.handle = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (nullptr == mWindow.handle)
	{
		glfwTerminate();
		assert(false && "Error creating context window.");
		return false;
	}

	glfwMakeContextCurrent(mWindow.handle);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(mWindow.handle);
		glfwTerminate();
		assert(false && "Error loading gl_core (ogl).\n");
		return false;
	}

	mCube.mesh = LoadMesh(BuildCube());
	mTestTri.mesh = LoadMesh(BuildTestTri());

	mShader.LoadShader("../Dryv3r/source/shaders/forwardVert.glsl", "../Dryv3r/source/shaders/forwardFrag.glsl");

	InitCamera();
	
	mShader.SetUniform("ViewProjection", Shader::MAT4, glm::value_ptr(mCamera.GetViewProjection()));
	mShader.SetUniform("Model", Shader::MAT4, glm::value_ptr(mCube.transform.GetTransform()));
	
	
	LoadTexture("../resources/textures/crate.png");
	mShader.SetUniform("albedoTexture", Shader::TEXTURE2D, (void*)&mTexture.name);
	return true;
}

bool Dryv3r::Update()
{
	if (glfwWindowShouldClose(mWindow.handle)) return false;
	glfwSwapBuffers(mWindow.handle);
	glfwPollEvents();
	return true;
}

void Dryv3r::Cleanup()
{
	//need buffer, texture cleanup implemented
}

void Dryv3r::Draw()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for each (GameObject* object in mObjectsToDraw)
	{
		DrawMesh(object->mesh);
	}
	mObjectsToDraw.clear();
}

void Dryv3r::DrawGameObject(GameObject& gameObject)
{
	mObjectsToDraw.push_back(&gameObject);
}

Texture* Dryv3r::LoadTexture(const char * filePath)
{
	mTexture = Texture();
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	const char* data = (const char*)stbi_load(filePath, &imageWidth, &imageHeight, &imageFormat, STBI_default);

	switch (imageFormat)
	{
	case 1: imageFormat = GL_RED; break;
	case 2: imageFormat = GL_RG; break;
	case 3: imageFormat = GL_RGB; break;
	case 4: imageFormat = GL_RGBA; break;
	}

	if (data == nullptr)
	{
		std::cout << "error loading texture.\n" << stbi_failure_reason();
		assert(false);
		return false;
	}

	mTexture = MakeTexture(imageWidth, imageHeight, imageFormat, GL_UNSIGNED_BYTE, data);
	stbi_image_free((void*)data);

	return &mTexture;
}

Mesh Dryv3r::LoadMesh(Geometry& geometry)
{
	Mesh newMesh;
	newMesh.count = geometry.indices.size();

	//create and bind vertex array
	glGenVertexArrays(1, &newMesh.vao);
	glBindVertexArray(newMesh.vao);

	//create and fill vertex buffer
	glGenBuffers(1, &newMesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, newMesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, geometry.vertices.size() * sizeof(Vertex), geometry.vertices.data(), GL_STATIC_DRAW);

	//create and fill index buffer
	glGenBuffers(1, &newMesh.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.indices.size() * sizeof(unsigned int), geometry.indices.data(), GL_STATIC_DRAW);

	//set vertex attributes
	glEnableVertexAttribArray(0);//position
	glEnableVertexAttribArray(1);//normal
	glEnableVertexAttribArray(2);//tangent
	glEnableVertexAttribArray(3);//UV coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(vec4) * 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4) * 3));

	//cleanup
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return newMesh;
}

void Dryv3r::DrawMesh(Mesh & mesh)
{
	glBindVertexArray(mesh.vao);
	glDrawElements(GL_TRIANGLES, mesh.count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Texture Dryv3r::MakeTexture(GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels)
{
	Texture newTexture;
	glGenTextures(1, &newTexture.name);
	glBindTexture(GL_TEXTURE_2D, newTexture.name);
	newTexture.width = width;
	newTexture.height = height;
	glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	return newTexture;
}

void Dryv3r::LoadTextureData(Texture & texture, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels)
{
	glBindTexture(GL_TEXTURE_2D, texture.name);
	glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, format, type, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Dryv3r::InitCamera()
{
	mCamera.SetPerspectiveProjection(45, (float)mWindow.width / mWindow.height, .1f, 1000.0f);
	mCamera.SetView(vec3(5), vec3(0), UP.xyz);
}
