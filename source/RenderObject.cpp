#include "RenderObject.h"

void RenderObject::LoadBuffers(Geometry & geometry)
{
	size = geometry.indecesSize;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, geometry.verticesSize * sizeof(Vertex), geometry.vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.indecesSize * sizeof(uint), geometry.indeces, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//bool RenderObject::LoadBuffer(Buffer_Type type, Buffer_Usage usage, const void* data, const uint dataSize, uint indexCount)
//{
//	uint* buffer;
//
//	bool success = true;
//	if (type == GL_ELEMENT_ARRAY_BUFFER)
//	{
//		buffer = &mVBO;
//		if (mVAO == 0)
//		{
//			glGenVertexArrays(1, &mVAO);
//			glBindVertexArray(mVAO);
//			glEnableVertexAttribArray(0);
//			glEnableVertexAttribArray(1);
//			//glEnableVertexAttribArray(2);
//			//glEnableVertexAttribArray(3);
//			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
//			//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));
//			//glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));
//			glBindVertexArray(0);
//		}
//	}
//	else
//	{
//		buffer = &mIBO;
//		mIndexCount = indexCount;
//	}
//	glGenBuffers(1, buffer);
//
//
//	glBindBuffer(type, *buffer);
//	glBufferData(type, dataSize, &data, usage);
//
//	GLint size = 0;
//	glGetBufferParameteriv(type, GL_BUFFER_SIZE, &size);
//	if (dataSize != size)
//	{
//		glDeleteBuffers(1, buffer);
//		std::cout << "Error loading buffer \n";
//		success = false;
//	}
//	glBindBuffer(type, 0);
//
//	return success;
//}
//
//void RenderObject::Draw()
//{
//	glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0);
//}
