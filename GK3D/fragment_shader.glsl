#version 150 core

in vec4 vertexColor;

void main()
{
    gl_FragColor = vertexColor;
}