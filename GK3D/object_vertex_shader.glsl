#version 150 core
  
in vec3 position;
in vec3 normal;

out vec3 vertexNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
	vertexNormal = normal;
}