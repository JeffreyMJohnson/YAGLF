#version 410
in vec2 vUV;

out vec4 FragColor;

uniform sampler2D albedoTexture;

void main() 
{
	FragColor = texture(albedoTexture, vUV);// vec4(0, .25f, 1, 1);
}
