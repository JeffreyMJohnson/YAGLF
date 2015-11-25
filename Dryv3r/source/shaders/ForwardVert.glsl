#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec4 Tangent;
layout(location = 3) in vec2 TexCoord;

out vec2 vUV;

uniform mat4 ViewProjection = mat4(1);
uniform mat4 Model = mat4(1);

void main()
{
	vUV = TexCoord;
	gl_Position = ViewProjection * Model * Position;
}
