#version 330

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D diffuse;
uniform sampler2D shadow;

void main() 
{

	
FragColor = texture(diffuse, vTexCoord) * texture(shadow, vTexCoord);

}