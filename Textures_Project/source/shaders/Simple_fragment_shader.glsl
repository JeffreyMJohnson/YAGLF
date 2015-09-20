#version 330

in vec4 vColor;

out vec4 FragColor;

uniform vec4 foo = vec4(0);

void main()
{
	FragColor = vColor;
}