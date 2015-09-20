#include "GLFramework.h"
#include "..\include\GLFramework.h"

Window* GLFramework::sWindow = new Window();
int GLFramework::counter = 0;
Camera* GLFramework::sCamera = new Camera();
Shader* GLFramework::sShader = new Shader();
std::vector<uint> GLFramework::sTextures;

//std::vector<RenderObject*> GLFramework::sRenderObjects = std::vector<RenderObject*>();
RenderObject* GLFramework::sRenderObject = nullptr;
bool GLFramework::useWireframe = false;

bool GLFramework::Startup(const int width, const int height, const char * title, const Color clearColor)
{
	if (!glfwInit())
	{
		return false;
	}
	sWindow->height = height;
	sWindow->width = width;
	sWindow->title = title;

	sWindow->handle = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (nullptr == sWindow->handle)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(sWindow->handle);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(sWindow->handle);
		glfwTerminate();
		return false;
	}

	SetClearColor(clearColor);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	return true;
}

bool GLFramework::SetShader(const char * vertexPath, const char * fragmentPath)
{
	return sShader->LoadShader(vertexPath, fragmentPath);
}

void GLFramework::SetShaderUniform(const char * name, const Shader::UniformType type, const void * value)
{
	sShader->SetUniform(name, type, value);
}

uint GLFramework::LoadTexture(const char * path)
{
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data = stbi_load(path, &imageWidth, &imageHeight, &imageFormat, STBI_default);	if (data == nullptr)	{		std::cout << "error loading texture.\n";	}	uint textureHandle;	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight,	0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);
	sTextures.push_back(textureHandle);
	return sTextures.size() - 1;
}

void GLFramework::SetTexture(Texture_Unit unit, uint texture)
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, sTextures[texture]);
}

void GLFramework::SetWireframe(bool value)
{
	useWireframe = value;
}

bool GLFramework::SetCameraView(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up)
{
	return sCamera->SetView(position, target, up);
}

bool GLFramework::SetCameraProjection(const float fov, const float aspectRatio, const float a_near, const float a_far)
{
	return sCamera->StartupPerspective(fov, aspectRatio, a_near, a_far);
}

void GLFramework::SlideCamera(const float hDistance, const float vDistance)
{
	sCamera->Slide(hDistance, vDistance);
}

void GLFramework::MoveCamera(const float distance)
{
	sCamera->Move(distance);
}

bool GLFramework::LoadModel(const char * path)
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
				geometry.vertices[i].color = xVert.colour;
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

	LoadModel(geometry);

	return true;
}

bool GLFramework::LoadModel(Geometry & geometry)
{
	LoadBuffers(geometry);
	return true;
}

bool GLFramework::Update()
{
	if (glfwWindowShouldClose(sWindow->handle) || glfwGetKey(sWindow->handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (useWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	sShader->SetUniform("ProjectionView", Shader::MAT4, glm::value_ptr(sCamera->GetViewProjection()));
	glBindVertexArray(sRenderObject->vao);
	glDrawElements(GL_TRIANGLES, sRenderObject->indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


	glfwSwapBuffers(sWindow->handle);
	glfwPollEvents();
	return true;
}

Color GLFramework::GetClearColor()
{
	Color result;
	if (nullptr != sWindow)
	{
		result = sWindow->clearColor;
	}
	return result;
}

void GLFramework::SetClearColor(const Color color)
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
	if (nullptr != sCamera)
	{
		delete sCamera;
	}
	if (nullptr != sShader)
	{
		delete sShader;
	}
	if (nullptr != sRenderObject)
	{
		sRenderObject->DeleteBuffers();
		delete sRenderObject;
	}


}

bool GLFramework::LoadBuffers(const Geometry & geometry)
{
	if (nullptr == sRenderObject)
	{
		sRenderObject = new RenderObject();
	}
	glGenVertexArrays(1, &sRenderObject->vao);
	glGenBuffers(1, &sRenderObject->vbo);
	glGenBuffers(1, &sRenderObject->ibo);

	glBindVertexArray(sRenderObject->vao);


	sRenderObject->indexCount = geometry.indices.size();

	glBindBuffer(GL_ARRAY_BUFFER, sRenderObject->vbo);
	glBufferData(GL_ARRAY_BUFFER, geometry.vertices.size() * sizeof(Vertex), geometry.vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sRenderObject->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.indices.size() * sizeof(uint), geometry.indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);//position
	glEnableVertexAttribArray(1);//color in shader right now.
	glEnableVertexAttribArray(2);//normal
	glEnableVertexAttribArray(3);//UV coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4) * 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(vec4) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4) * 3));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}
