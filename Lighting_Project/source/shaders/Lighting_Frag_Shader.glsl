#version 330

in vec4 vNormal;

out vec4 FragColor;

uniform vec3 uLightDirection = vec3(0,1,0);
uniform vec3 uLightColor = vec3(.99f,.72f,.07f);

void main() 
{
	float diffuse = max(0, dot(normalize(vNormal.xyz), uLightDirection));
	FragColor = vec4(diffuse,diffuse,diffuse,1) * vec4(uLightColor.xyz, 1);
}