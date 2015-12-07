#version 150 core

in vec3 vertexNormal;
in vec3 vertexFragPos;
out vec4 color;

uniform vec3 viewPos;
uniform samplerCube envTexture;

void main()
{             
    vec3 I = normalize(vertexFragPos - viewPos);
    vec3 R = reflect(I, normalize(vertexNormal));
    color = texture(envTexture, R);
}