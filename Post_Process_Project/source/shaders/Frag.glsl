#version 410

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D target;

vec4 Simple()
{
	return texture(target, vTexCoord);
}

// simple box blur
vec4 BoxBlur() 
{
	vec2 texel = 1.0f / textureSize(target, 0).xy;
	// 9-tap box kernel
	vec4 color = texture(target, vTexCoord);
	color += texture(target, vTexCoord + vec2(-texel.x, texel.y));
	color += texture(target, vTexCoord + vec2(-texel.x, 0));
	color += texture(target, vTexCoord + vec2(-texel.x, -texel.y));
	color += texture(target, vTexCoord + vec2(0, texel.y));
	color += texture(target, vTexCoord + vec2(0, -texel.y));
	color += texture(target, vTexCoord + vec2(texel.x, texel.y));
	color += texture(target, vTexCoord + vec2(texel.x, 0));
	color += texture(target, vTexCoord + vec2(texel.x, -texel.y));
	return color / 9;
}

vec4 Distort() 
{
	vec2 mid = vec2(0.5f);
	float distanceFromCentre = distance(vTexCoord, mid);
	vec2 normalizedCoord = normalize(vTexCoord - mid);
	float bias = distanceFromCentre +
	sin(distanceFromCentre * 15) * 0.02f;
	vec2 newCoord = mid + bias * normalizedCoord;
	return texture(target, newCoord);
}


void main() 
{
	//FragColor = Simple();
	//FragColor = BoxBlur();
	FragColor = Distort();
}