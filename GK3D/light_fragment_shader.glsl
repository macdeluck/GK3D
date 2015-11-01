#version 150 core
out vec4 color;

uniform vec3 intensity;

void main()
{
    color = vec4(intensity, 1.0f);
}