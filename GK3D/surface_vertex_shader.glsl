#version 150 core
  
in vec3 position;
in vec3 normal;

out vec3 vertexNormal;
out vec3 vertexFragPos;
out vec2 vertexTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 texModel;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vertexFragPos = vec3(model * vec4(position, 1.0f));
	vertexNormal = mat3(transpose(inverse(model))) * normal;
	vertexTexCoord = (texModel * vec3(vertexFragPos.xz, 1.0f)).xy;
}