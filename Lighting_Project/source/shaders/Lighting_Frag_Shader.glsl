#version 330

in vec4 vPosition;
in vec4 vNormal;

out vec4 FragColor;

uniform vec3 lightDirection = vec3(0,1,0);
uniform vec3 lightColor = vec3(.99f,.72f,.07f);
uniform vec3 cameraPosition = vec3(10,10,10);
uniform float specularPower = 1.0f;

void main() 
{
	float diffuse = max(0, dot(normalize(vNormal.xyz), lightDirection));
	vec3 eyeDirection = normalize(cameraPosition - vPosition.xyz);
	vec3 reflectedLight = reflect(-lightDirection, vNormal.xyz);
	float specular = max(0, dot(eyeDirection, reflectedLight));
	specular = pow(specular, specularPower);
	
	FragColor = vec4(lightColor * diffuse + lightColor * specular, 1);
}