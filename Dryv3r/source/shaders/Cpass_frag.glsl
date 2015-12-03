#version 410

in vec2 vTexCoord;

out vec4 FragColor;


uniform sampler2D Albedo;
uniform sampler2D Light;

uniform bool UseLight = true;
uniform sampler2D DebugTexture;

void main()
{
	vec3 light = texture(Light, vTexCoord).rgb;
	vec3 albedo = texture(Albedo, vTexCoord).rgb;
	if (UseLight)
	{
		FragColor = vec4(albedo * light, 1);
	}
	else
	{
		FragColor = vec4(albedo, 1);
	}
	//FragColor = vec4(light, 1);
}