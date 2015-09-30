#version 330

layout(location=0) in vec4 Position;
layout(location=4) in vec2 TexCoord;

out vec2 vTexCoord;

uniform mat4 ProjectionView;

void main() 
{
	vTexCoord = TexCoord;
	gl_Position= ProjectionView * Position;
}