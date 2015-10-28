#version 150 core
  
in vec3 position;
in vec3 color;

out vec3 vertexColor;
uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 1.0f);
    vertexColor = color;
}