#version 330

in vec4 vPosition;
in vec4 vNormal;

out vec4 FragColor;

//light properties
uniform vec3 lightDirection = vec3(0,1,0);
uniform vec3 lightAmbient = vec3(.99f,.72f,.07f);
uniform vec3 lightDiffuse = vec3(0,0,1);
uniform vec3 lightSpecular = vec3(1,1,1);
uniform vec3 lightPosition = vec3(0);
uniform vec3 cameraPosition = vec3(10,10,10);
uniform float specularPower = 1.0f;

//Ka * Ia + sum(Kd*(dot(Lm, N)* Id + Ks * dot(Rm, V)^a * Is)
//Ka = materials ambient color
//Ia = Lights ambient color
//Kd = materials diffuse color
//Lm = lights direction
//N = surface normal vector
//Id = lights diffuse color
//Ks = materials specular color
//Rm = reflected light direction
//V = view direction (normalize(camera position - vertex position))
//a = specular power
//Is = lights specular color

//ambient + sum (diffuse + specular)
//ambient = Ka * Ia
//diffuse = Kd * (dot(Lm, N) * Id)
//specular = Ks * (dot(Rm, V) * Is)

void main() 
{
	float diffuse = max(0, dot(normalize(vNormal.xyz), lightDirection));
	vec3 eyeDirection = normalize(cameraPosition - vPosition.xyz);
	vec3 reflectedLight = reflect(-lightDirection, vNormal.xyz);
	float specular = max(0, dot(eyeDirection, reflectedLight));
	specular = pow(specular, specularPower);
	
	FragColor = vec4(lightColor * diffuse + lightColor * specular, 1);
}