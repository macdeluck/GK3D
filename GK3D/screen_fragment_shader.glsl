#version 150 core
in vec2 vertexTexCoords;

out vec4 color;

uniform sampler2D screenTexture;

void main()
{    
    color = vec4(vec3(texture(screenTexture, vertexTexCoords)), 1.0f);
}