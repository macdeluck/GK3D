#version 150 core

in vec3 vertexNormal;
in vec3 vertexFragPos; 

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
out vec4 color;

void main()
{
	vec3 lightDir = normalize(lightPos - vertexFragPos);

	// ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 norm = normalize(vertexNormal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - vertexFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  

	// result
    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0f);
}