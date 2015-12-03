#version 150 core
out vec4 color;
in vec3 vertexFragPos;
uniform vec3 viewPos;

uniform vec3 intensity;

vec3 CalcFogImpact(vec3 color)
{
	float fogDensity = 0.3;
	float fogFactor;
    float dist = length(viewPos - vertexFragPos);
	vec3 fogColor = vec3(0.1, 0.1, 0.1);
	fogFactor = 1.0 /exp(dist * fogDensity);
    fogFactor = clamp(fogFactor, 0.0, 1.0);
	return (1 - fogFactor) * fogColor + fogFactor * color;
}

void main()
{
    color = vec4(CalcFogImpact(intensity), 1.0f);
}