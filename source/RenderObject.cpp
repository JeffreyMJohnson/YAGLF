#include "RenderObject.h"

bool RenderObject::LoadBuffer(Buffer_Type type, Buffer_Usage usage, const std::vector<float>& data)
{
	uint* buffer;
	bool success = true;
	if (type == GL_ELEMENT_ARRAY_BUFFER)
	{
		buffer = &mVBO;
	}
	else
	{
		buffer = &mIBO;
	}
	glGenBuffers(1, buffer);

	glBindBuffer(type, *buffer);
	glBufferData(type, sizeof(float), &data, usage);

	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	if (sizeof(float) != size)
	{
		glDeleteBuffers(1, buffer);
		std::cout << "Error loading buffer \n";
		success = false;
	}
	glBindBuffer(type, 0);

	return success;
}
