#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec4 Normal;
layout(location = 3) in vec4 Tangent;
layout(location = 4) in vec2 TexCoord;

// view-space normal and position
out vec4 vPosition;
out vec4 vNormal;
out vec2 vTexCoord;

uniform mat4 View; // needed in addition to Projection * View
uniform mat4 ProjectionView;


void main() 
{
	// first store view-space position and normal
	vPosition = View * Position;
	vNormal = normalize(View * Normal);
	vTexCoord = TexCoord;
	gl_Position = ProjectionView * Position;
}