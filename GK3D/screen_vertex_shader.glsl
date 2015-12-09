#version 150 core
in vec3 position;
in vec2 texCoords;

out vec2 vertexTexCoords;

void main()
{
	gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
	vertexTexCoords = texCoords;
}