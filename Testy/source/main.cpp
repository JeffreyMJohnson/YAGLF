#include "GLFramework.h"
#include <iostream>

typedef GLFramework glf;
typedef glm::vec3 vec3;


Geometry BuildGrid()
{
	int rows = 25, cols = 25;
	uint verticesSize = rows * cols;
	Geometry grid;
	grid.vertices.resize(verticesSize);
	for (uint r = 0; r < rows; r++)
	{
		for (uint c = 0; c < cols; c++)
		{
			grid.vertices[r * cols + c].position = vec4((float)c, 0, (float)r, 1);

			vec3 color = vec3(sinf((c / (float)(cols - 1))*(r / (float)(rows - 1))));

			grid.vertices[r*cols + c].color = vec4(color, 1);
		}
	}



	uint indecesCount = (rows - 1) * (cols - 1) * 6;
	grid.indices.resize(indecesCount);

	uint index = 0;
	for (uint r = 0; r < (rows - 1); r++)
	{
		for (uint c = 0; c < (cols - 1); c++)
		{
			//triangle 1
			grid.indices[index++] = r*cols + c;
			grid.indices[index++] = (r + 1)*cols + c;
			grid.indices[index++] = (r + 1)*cols + (c + 1);

			//triangle 2
			grid.indices[index++] = r*cols + c;
			grid.indices[index++] = (r + 1)*cols + (c + 1);
			grid.indices[index++] = r*cols + (c + 1);
		}
	}
	return grid;
}

Geometry BuildQuad()
{
	Geometry quad;
	Vertex v1;
	v1.position = vec4(-5, 0, 5, 1);
	v1.UV = vec2(0, 1);
	quad.vertices.push_back(v1);
	Vertex v2;
	v2.position = vec4(5, 0, 5, 1);
	v2.UV = vec2(1, 1);
	quad.vertices.push_back(v2);
	Vertex v3;
	v3.position = vec4(5, 0, -5, 1);
	v3.UV = vec2(1, 0);
	quad.vertices.push_back(v3);
	Vertex v4;
	v4.position = vec4(-5, 0, -5, 1);
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

void main()
{
	using namespace std;

	bool start = glf::Startup(1280, 720, "foo", Color(.5f, .5f, .5f, 1));
	if (!start)
	{
		std::cout << "Failed startup.\n";
	}
	else
	{
		std::cout << "Startup succeded.\n";
	}

	bool shader = glf::SetShader("../Testy/source/Texture_Vert_shader.glsl", "../Testy/source/Texture_Frag_Shader.glsl");
	
	if (!shader)
	{
		std::cout << "Failed loading shaders.\n";
	}
	else
	{
		std::cout << "Loading shaders succeded.\n";
	}

	uint texture = glf::LoadTexture("../resources/textures/crate.png");
	uint shadow = glf::LoadTexture("../resources/textures/star.png");
	
		glf::SetTexture(Texture_Unit::ZERO, texture);
		int texNum = 0;
		glf::SetShaderUniform("diffuse", Shader::INT1, &texNum);
		
		int shadowNum = 1;
		glf::SetTexture(Texture_Unit::ONE, shadow);
		glf::SetShaderUniform("shadow", Shader::INT1, &shadowNum);

	bool camera = glf::SetCameraProjection(glm::pi<float>() * .25f, (float)1280 / 720, .1f, 1000.0f);
	camera = glf::SetCameraView(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	if (!camera)
	{
		std::cout << "Failed loading camera.\n";
	}
	else
	{
		std::cout << "Loading camera succeded.\n";
	}


	//glf::SetWireframe(true);
	//Geometry grid = BuildGrid();
	//glf::LoadModel(grid);
	//glf::LoadModel("../Testy/resources/models/soulspear/soulspear.fbx");
	glf::LoadModel(BuildQuad());
	float count = 0;

	while (glf::Update())
	{
		
	}
	//system("pause");
	glf::Cleanup();
}