#version 150 core

in vec3 vertexNormal;
in vec3 vertexFragPos; 

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
out vec4 color;

void main()
{
	// ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 norm = normalize(vertexNormal);
	vec3 lightDir = normalize(lightPos - vertexFragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// result
    vec3 result = (ambient + diffuse) * objectColor;
    color = vec4(result, 1.0f);
}