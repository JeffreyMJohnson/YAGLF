#include "GLFramework.h"

Window* GLFramework::sWindow = new Window();
int GLFramework::counter = 0;
std::vector<Shader*> GLFramework::sShaders { nullptr };
std::vector<uint> GLFramework::sTextures;
//std::vector<RenderObject*> GLFramework::sRenderObjects = std::vector<RenderObject*>();
std::vector<RenderObject> GLFramework::sRenderObjects;
bool GLFramework::useWireframe = false;
Timer GLFramework::sTimer;
std::vector<VertexAttribute> sVertexAttributes;

bool GLFramework::Startup(const int width, const int height, const char * title, const vec4 clearColor)
{
	if (!glfwInit())
	{
		assert(false && "Error initializing GLFW.");
		return false;
	}
	sWindow->height = height;
	sWindow->width = width;
	sWindow->title = title;

	sWindow->handle = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (nullptr == sWindow->handle)
	{
		glfwTerminate();
		assert(false && "Error creating context window.");
		return false;
	}

	glfwMakeContextCurrent(sWindow->handle);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(sWindow->handle);
		glfwTerminate();
		assert(false && "Error loading gl_core (ogl).\n");
		return false;
	}

	SetClearColor(clearColor);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	Keyboard::Init();

	//load the premade models
	//note the order of this must be same as Model enum order...
	LoadModel(BuildCube());

	LoadVertexAttributes()

	return true;
}

uint GLFramework::CreateQuad()
{
	return LoadModel(BuildQuad());
}

uint GLFramework::LoadShader(std::string vertexPath, std::string fragmentPath)
{
	Shader* s = new Shader();
	if (!s->LoadShader(vertexPath, fragmentPath))
	{
		delete s;
		return 0;
	}
	sShaders.push_back(s);
	glUseProgram(s->GetProgram());
	return sShaders.size() - 1;

}

void GLFramework::UseShader(uint shader)
{
	glUseProgram(sShaders[shader]->GetProgram());
}

void GLFramework::SetShaderUniform(const unsigned int shader, std::string varName, const Shader::UniformType type, const void* value, const uint count)
{
	sShaders[shader]->SetUniform(varName.c_str(), type, value, count);
}

//bool GLFramework::SetShader(const char * vertexPath, const char * fragmentPath)
//{
//	return sShader->LoadShader(vertexPath, fragmentPath);
//}

//void GLFramework::SetShaderUniform(const char * name, const Shader::UniformType type, const void * value)
//{
//	sShader->SetUniform(name, type, value);
//}

uint GLFramework::LoadTexture(const char * path)
{
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data = stbi_load(path, &imageWidth, &imageHeight, &imageFormat, STBI_default);

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
	}
	uint textureHandle;
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, imageWidth, imageHeight,0, imageFormat, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);
	sTextures.push_back(textureHandle);
	return sTextures.size() - 1;
}

//void GLFramework::SetTexture(Texture_Unit unit, uint texture)
//{
//	glActiveTexture(unit);
//	glBindTexture(GL_TEXTURE_2D, sTextures[texture]);
//}

void GLFramework::SetWireframe(bool value)
{
	if (value)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
}

//bool GLFramework::SetCameraView(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up)
//{
//	return sCamera->SetView(position, target, up);
//}
//
//bool GLFramework::SetCameraProjection(const float fov, const float aspectRatio, const float a_near, const float a_far)
//{
//	return sCamera->SetPerspectiveProjection(fov, aspectRatio, a_near, a_far);
//}
//
//void GLFramework::SlideCamera(const float hDistance, const float vDistance)
//{
//	sCamera->Slide(hDistance, vDistance);
//}
//
//void GLFramework::MoveCamera(const float distance)
//{
//	sCamera->Move(distance);
//}

uint GLFramework::LoadModel(const char * path)
{
	bool success = true;
	//find extension
	std::string sPath(path);
	std::string ext = sPath.substr(sPath.find_last_of('.'));

	Geometry geometry;

	if (ext == ".obj")
	{
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err = tinyobj::LoadObj(shapes, materials, path);
		if (err.length() != 0)
		{
			std::cout << "Error loading OBJ file:\n" << err << std::endl;
			success = false;
		}

		//hard coding only using first shape, can change to loop here
		if (success)
		{
			auto shape = shapes[0];
			auto mesh = shape.mesh;

			geometry.vertices.resize(mesh.positions.size());

			uint posIndex = 0;
			uint normalIndex = 0;
			uint UVIndex = 0;
			bool hasNormals = mesh.normals.size() == mesh.positions.size();
			bool hasUVs = mesh.texcoords.size() == mesh.positions.size();
			//obj has vectors of floats, my struct and shaders uses glm vecs so need to build myself
			for (uint vertexCount = 0; posIndex < mesh.positions.size(); vertexCount++)
			{
				float x = mesh.positions[posIndex++];
				float y = mesh.positions[posIndex++];
				float z = mesh.positions[posIndex++];
				geometry.vertices[vertexCount].position = vec4(x, y, z, 1);

				if (hasNormals)
				{
					x = mesh.normals[normalIndex++];
					y = mesh.normals[normalIndex++];
					z = mesh.normals[normalIndex++];
					geometry.vertices[vertexCount].normal = vec4(x, y, z, 1);
				}

				if (hasUVs)
				{
					x = mesh.texcoords[UVIndex++];
					y = mesh.texcoords[UVIndex++];
					geometry.vertices[vertexCount].UV = vec2(x, y);
				}
			}

			geometry.indices = mesh.indices;
		}
	}
	else if (ext == ".fbx")
	{


		FBXFile file;
		success = file.load(path, FBXFile::UNITS_METER, false, false, false);
		if (!success)
		{
			std::cout << "Error loading FBX file:\n";
		}
		else
		{
			//hardcoding to use single mesh, can loop here if needed.
			FBXMeshNode* mesh = file.getMeshByIndex(0);
			geometry.vertices.resize(mesh->m_vertices.size());

			for (int i = 0; i < mesh->m_vertices.size(); i++)
			{
				auto xVert = mesh->m_vertices[i];
				geometry.vertices[i].position = xVert.position;
				geometry.vertices[i].normal = xVert.normal;
				geometry.vertices[i].UV = xVert.texCoord1;
			}

			geometry.indices = mesh->m_indices;

			file.unload();
		}

	}
	else
	{
		std::cout << "Unsupported format. Only support .obj or .fbx files.\n";
		success = false;
	}
	if (!success)
	{
		return false;
	}

	return LoadModel(geometry);
}

uint GLFramework::LoadModel(Geometry& geometry)
{
	return MakeVAO(geometry);
}

void GLFramework::DrawModel(uint modelID)
{
	DrawVAO(modelID);
}

bool GLFramework::Update()
{
	sTimer.Update();
	glfwSwapBuffers(sWindow->handle);
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT);
	return true;
}

vec4 GLFramework::GetClearColor()
{
	vec4 result;
	if (nullptr != sWindow)
	{
		result = sWindow->clearColor;
	}
	return result;
}

void GLFramework::SetClearColor(const vec4 color)
{
	if (nullptr != sWindow)
	{
		sWindow->clearColor = color;
		glClearColor(sWindow->clearColor.r, sWindow->clearColor.g, sWindow->clearColor.b, sWindow->clearColor.a);
	}
}

void GLFramework::Cleanup()
{
	if (nullptr != sWindow)
	{
		delete sWindow;
	}

	sRenderObjects.clear();

	for (int i = 1; i < sShaders.size(); i++)
	{
		delete sShaders[i];
	}
	sShaders.clear();
}

//bool GLFramework::LoadBuffers(const Geometry & geometry)
//{
//	RenderObject rendObj;
//	glGenVertexArrays(1, &rendObj.vao);
//	glGenBuffers(1, &rendObj.vbo);
//	glGenBuffers(1, &rendObj.ibo);
//
//	glBindVertexArray(rendObj.vao);
//
//
//	rendObj.indexCount = geometry.indices.size();
//
//	glBindBuffer(GL_ARRAY_BUFFER, rendObj.vbo);
//	glBufferData(GL_ARRAY_BUFFER, geometry.vertices.size() * sizeof(Vertex), geometry.vertices.data(), GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendObj.ibo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.indices.size() * sizeof(uint), geometry.indices.data(), GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(0);//position
//	glEnableVertexAttribArray(1);//color in shader right now.
//	glEnableVertexAttribArray(2);//normal
//	glEnableVertexAttribArray(3);//tangent
//	glEnableVertexAttribArray(4);//UV coord
//
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4) * 1));
//	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(vec4) * 2));
//	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4) * 3));
//	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4) * 4));
//
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	sRenderObjects.push_back(rendObj);
//	return sRenderObjects.size()-1;
//}

//void GLFramework::UpdateFlyCamControls()
//{
//	if (Keyboard::IsKeyPressed(Keyboard::KEY_W) || Keyboard::IsKeyRepeat(Keyboard::KEY_W))
//	{
//		MoveCamera(1);
//	}
//	if (Keyboard::IsKeyPressed(Keyboard::KEY_X) || Keyboard::IsKeyRepeat(Keyboard::KEY_X))
//	{
//		MoveCamera(-1);
//	}
//	if (Keyboard::IsKeyPressed(Keyboard::KEY_A) || Keyboard::IsKeyRepeat(Keyboard::KEY_A))
//	{
//		SlideCamera(-1, 0);
//	}
//	if (Keyboard::IsKeyPressed(Keyboard::KEY_D) || Keyboard::IsKeyRepeat(Keyboard::KEY_D))
//	{
//		SlideCamera(1, 0);
//	}
//	if (Keyboard::IsKeyPressed(Keyboard::KEY_E) || Keyboard::IsKeyRepeat(Keyboard::KEY_E))
//	{
//		SlideCamera(0, 1);
//	}
//	if (Keyboard::IsKeyPressed(Keyboard::KEY_C) || Keyboard::IsKeyRepeat(Keyboard::KEY_C))
//	{
//		SlideCamera(0,-1);
//	}
//}

Geometry GLFramework::BuildQuad()
{
	Geometry quad;
	Vertex v1;
	v1.position = vec4(-5, 0, 5, 1);
	v1.normal = vec4(0, 1, 0, 0);
	v1.tangent = vec4(1, 0, 0, 0);
	v1.UV = vec2(0, 1);
	quad.vertices.push_back(v1);
	Vertex v2;
	v2.position = vec4(5, 0, 5, 1);
	v2.normal = vec4(0, 1, 0, 0);
	v2.tangent = vec4(1, 0, 0, 0);
	v2.UV = vec2(1, 1);
	quad.vertices.push_back(v2);
	Vertex v3;
	v3.position = vec4(5, 0, -5, 1);
	v3.normal = vec4(0, 1, 0, 0);
	v3.tangent = vec4(1, 0, 0, 0);
	v3.UV = vec2(1, 0);
	quad.vertices.push_back(v3);
	Vertex v4;
	v4.position = vec4(-5, 0, -5, 1);
	v4.normal = vec4(0, 1, 0, 0);
	v4.tangent = vec4(1, 0, 0, 0);
	v4.UV = vec2(0, 0);
	quad.vertices.push_back(v4);

	quad.indices.push_back(0);
	quad.indices.push_back(1);
	quad.indices.push_back(2);
	quad.indices.push_back(0);
	quad.indices.push_back(2);
	quad.indices.push_back(3);

	return quad;
}

/*
Creates and fills vbo and ibo buffers with given geometry data. returns the index of render object in list.
*/
uint GLFramework::MakeVAO(Geometry & geometry)
{
	//create new render object to hold handles
	RenderObject rendObj;
	rendObj.indexCount = geometry.indices.size();
	//create and bind vertex array
	glGenVertexArrays(1, &rendObj.vao);
	glBindVertexArray(rendObj.vao);

	//create and fill vertex buffer
	glGenBuffers(1, &rendObj.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, rendObj.vbo);
	glBufferData(GL_ARRAY_BUFFER, geometry.vertices.size() * sizeof(Vertex), geometry.vertices.data(), GL_STATIC_DRAW);

	//create and fill index buffer
	glGenBuffers(1, &rendObj.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendObj.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.indices.size() * sizeof(uint), geometry.indices.data(), GL_STATIC_DRAW);

	//set vertex attributes
	glEnableVertexAttribArray(0);//position
	glEnableVertexAttribArray(1);//normal
	glEnableVertexAttribArray(2);//tangent
	glEnableVertexAttribArray(3);//UV coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));

	//cleanup
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//add the new render object to list and return the index.
	sRenderObjects.push_back(rendObj);
	return sRenderObjects.size() - 1;
}

/*
Calls GL draw call with given renderObject
*/
void GLFramework::DrawVAO(uint renderObject)
{
	glBindVertexArray(sRenderObjects[renderObject].vao);
	glDrawElements(GL_TRIANGLES, sRenderObjects[renderObject].indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/*
Cleans up the created GL buffrers in given render object
*/
void GLFramework::KillVAO(uint renderObject)
{
	glDeleteBuffers(1, &(sRenderObjects[renderObject].vbo));
	glDeleteBuffers(1, &sRenderObjects[renderObject].ibo);
	glDeleteVertexArrays(1, &sRenderObjects[renderObject].vao);
}

void GLFramework::LoadVertexAttributes()
{
	VertexAttribute v0(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	VertexAttribute v1(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 1));
	VertexAttribute v2(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));
	VertexAttribute v3(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));
	sVertexAttributes.push_back(v0);
	sVertexAttributes.push_back(v1);
	sVertexAttributes.push_back(v2);
	sVertexAttributes.push_back(v3);
}
