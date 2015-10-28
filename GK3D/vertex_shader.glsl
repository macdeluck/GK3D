#version 150 core
  
in vec3 position;
out vec4 vertexColor;

uniform vec4 timeColor;
void main()
{
    gl_Position = vec4(position, 1.0);
	vertexColor = timeColor;
}