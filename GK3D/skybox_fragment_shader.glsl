#version 150 core
out vec4 color;
in vec3 vertexTexCoord;

uniform samplerCube skyboxTexture;

void main()
{    
    color = texture(skyboxTexture, vertexTexCoord);
}