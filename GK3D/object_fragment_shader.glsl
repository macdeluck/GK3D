#version 150 core

uniform vec3 objectColor;
out vec4 color;

void main()
{
    color = vec4(objectColor, 1.0f);
}