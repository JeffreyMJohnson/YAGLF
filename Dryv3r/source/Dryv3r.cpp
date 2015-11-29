#include "Dryv3r.h"

Window Dryv3r::mWindow;
std::vector<Mesh> Dryv3r::mMeshList;
std::vector<Texture> Dryv3r::mTextureList;
std::vector<GameObject*> Dryv3r::mObjectsToDraw;
Shader Dryv3r::mForwardPassShader;
Shader Dryv3r::mGpassShader;
Shader Dryv3r::mCpassShader;
Camera Dryv3r::mCamera;
uint  Dryv3r::albedoTexture;
uint  Dryv3r::positionTexture;
uint  Dryv3r::normalTexture;
uint Dryv3r::depthTexture;
uint Dryv3r::mGpassFrameBuffer = 0;

const uint Dryv3r::MESH_ID_CUBE = 0;
const uint Dryv3r::MESH_ID_QUAD = 1;
const uint Dryv3r::TEXTURE_ID_DEFAULT = 0;

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

	//init stock mesh's
	LoadMesh(BuildCube());
	LoadMesh(BuildQuad());

	InitCamera();

	//make default texture with one white pixel
	MakeTexture(1, 1, GL_RGBA, GL_FLOAT, glm::value_ptr(vec4(1,1,0,1)));

	//load shader for forward render pass
	mForwardPassShader.LoadShader("../Dryv3r/source/shaders/forwardVert.glsl", "../Dryv3r/source/shaders/forwardFrag.glsl");

	//load shader for render passes
	mGpassShader.LoadShader("../Dryv3r/source/shaders/Gpass_vert.glsl", "../Dryv3r/source/shaders/Gpass_frag.glsl");
	mCpassShader.LoadShader("../Dryv3r/source/shaders/Cpass_vert.glsl", "../Dryv3r/source/shaders/Cpass_frag.glsl");
	InitGpass();
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

	//prep gpass
	glBindFramebuffer(GL_FRAMEBUFFER, mGpassFrameBuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(.25f,.25f,.25f,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(mGpassShader.GetProgram());

	//draw the gpass
	mGpassShader. SetUniform("Projection", Shader::MAT4, glm::value_ptr(mCamera.GetProjection()));
	mGpassShader. SetUniform("View", Shader::MAT4, glm::value_ptr(mCamera.GetView()));

	for each (GameObject* object in mObjectsToDraw)
	{
		mGpassShader. SetUniform("Model", Shader::MAT4, glm::value_ptr(object->transform.GetTransform()));
		mGpassShader. SetUniform("Diffuse", Shader::TEXTURE2D, (void*)&mTextureList[object->diffuse]);
		DrawMesh(mMeshList[object->mesh]);
	}
	mObjectsToDraw.clear();

	//cleanup gpass
	glDisable(GL_DEPTH_TEST);

	//prep cpass
	glClearColor(1, 0, 0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(mCpassShader.GetProgram());

	//draw cpass
	//frag shader uniforms
	mCpassShader.SetUniform("Albedo", Shader::TEXTURE2D, (void*)&mTextureList[albedoTexture]);
	//mCpassShader.SetUniform("Light", Shader::TEXTURE2D, (void*)&mTextureList[albedoTexture]);

	DrawMesh(mMeshList[MESH_ID_QUAD]);

	//cleanup cpass
	glUseProgram(0);
	glBindVertexArray(0);

}

void Dryv3r::DrawGameObject(GameObject& gameObject)
{
	mObjectsToDraw.push_back(&gameObject);
}

GameObject Dryv3r::GetCube()
{
	GameObject newObject;
	newObject.mesh = MESH_ID_CUBE;
	//diffuse texture is default 0 and that is what we want
	return newObject;
}

GameObject Dryv3r::GetQuad()
{
	GameObject newObject;
	newObject.mesh = MESH_ID_QUAD;
	return newObject;
}

uint Dryv3r::LoadTexture(const char * filePath)
{
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

	uint texture = MakeTexture(imageWidth, imageHeight, imageFormat, GL_UNSIGNED_BYTE, data);
	stbi_image_free((void*)data);

	return texture;
}

uint Dryv3r::LoadMesh(Geometry& geometry)
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

	mMeshList.push_back(newMesh);
	return mMeshList.size() - 1;
}

void Dryv3r::DrawMesh(Mesh & mesh)
{
	glBindVertexArray(mesh.vao);
	glDrawElements(GL_TRIANGLES, mesh.count, GL_UNSIGNED_INT, 0);
}

uint Dryv3r::MakeTexture(GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels)
{
	Texture newTexture;
	glGenTextures(1, &newTexture.name);
	glBindTexture(GL_TEXTURE_2D, newTexture.name);
	newTexture.width = width;
	newTexture.height = height;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	mTextureList.push_back(newTexture);
	return mTextureList.size() - 1;
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

void Dryv3r::InitGpass()
{
	/*
	 "GPassAlbedo","GPassPosition","GPassNormal","GPassDepth" };
	const unsigned gpassDepths[] = { GL_RGB8,GL_RGB32F,GL_RGB32F,GL_DEPTH_COMPONENT
	*/
	mGpassFrameBuffer;
	glGenFramebuffers(1, &mGpassFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mGpassFrameBuffer);

	//make textures
	/*
	albedoTexture = MakeTexture(mWindow.width, mWindow.height, GL_RGB, GL_FLOAT, nullptr);
	positionTexture = MakeTexture(mWindow.width, mWindow.height, GL_RGB, GL_FLOAT, nullptr);
	normalTexture = MakeTexture(mWindow.width, mWindow.height, GL_RGB, GL_FLOAT, nullptr);
	depthTexture = MakeTexture(mWindow.width, mWindow.height, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	*/
	Texture albedo{ 0, mWindow.width, mWindow.height };
	glGenTextures(1, &albedo.name);
	glBindTexture(GL_TEXTURE_2D, albedo.name);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, mWindow.width, mWindow.height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, albedo.name, 0);
	mTextureList.push_back(albedo);
	albedoTexture = mTextureList.size() - 1;

	Texture position{ 0, mWindow.width, mWindow.height };
	glGenTextures(1, &position.name);
	glBindTexture(GL_TEXTURE_2D, position.name);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, mWindow.width, mWindow.height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, position.name, 0);
	mTextureList.push_back(position);
	positionTexture = mTextureList.size() - 1;

	Texture normal{ 0, mWindow.width, mWindow.height };
	glGenTextures(1, &normal.name);
	glBindTexture(GL_TEXTURE_2D, normal.name);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, mWindow.width, mWindow.height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, normal.name, 0);
	mTextureList.push_back(normal);
	normalTexture = mTextureList.size() - 1;

	Texture depth{ 0, mWindow.width, mWindow.height };
	glGenTextures(1, &depth.name);
	glBindTexture(GL_TEXTURE_2D, depth.name);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mWindow.width, mWindow.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth.name, 0);
	mTextureList.push_back(depth);
	depthTexture = mTextureList.size() - 1;

	//draw buffers
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, drawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		bool incompleteAttachment = status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
		bool invalidEnum = status == GL_INVALID_ENUM;
		bool invalidValue = status == GL_INVALID_VALUE;
		printf("Framebuffer Error!\n");
		assert(false);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
