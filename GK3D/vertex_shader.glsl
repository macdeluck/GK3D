#version 150 core
  
in vec3 position;
in vec3 color;

out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = vec4(position, 1.0f);
    vertexColor = color;
}