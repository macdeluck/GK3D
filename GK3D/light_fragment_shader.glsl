#version 150 core
out vec4 color;
in vec3 vertexFragPos;
uniform vec3 viewPos;

uniform vec3 intensity;

struct Fog
{
	float density;
	vec3 color;
};

uniform Fog fog;

const float lightFogImpact = 0.75;

vec3 CalcFogImpact(vec3 color)
{
	float fogFactor;
    float dist = length(viewPos - vertexFragPos);
	fogFactor = 1.0 /exp(dist * fog.density * lightFogImpact);
    fogFactor = clamp(fogFactor, 0.0, 1.0);
	return (1 - fogFactor) * fog.color + fogFactor * color;
}

void main()
{
    color = vec4(CalcFogImpact(intensity), 1.0f);
}