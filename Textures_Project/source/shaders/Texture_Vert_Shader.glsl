#version 330

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Color;
layout(location=2) in vec4 Normal;
layout(location=3) in vec4 Tangent;
layout(location=4) in vec2 TexCoord;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiTangent;

uniform mat4 ProjectionView;

void main() 
{
	vTexCoord = TexCoord;
	vNormal = Normal.xyz;
	vTangent = Tangent.xyz;
	vBiTangent = cross(vNormal, vTangent);
	gl_Position= ProjectionView * Position;
}