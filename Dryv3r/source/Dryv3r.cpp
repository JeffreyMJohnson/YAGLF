#include "Dryv3r.h"

Window Dryv3r::mWindow;
GameObject Dryv3r::mCube;
GameObject Dryv3r::mTestTri;
std::vector<GameObject*> Dryv3r::mObjectsToDraw;
Shader Dryv3r::mShader;


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
}

void Dryv3r::Draw()
{

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
