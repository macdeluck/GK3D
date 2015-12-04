#version 150 core
  
in vec3 position;
out vec3 vertexTexCoord;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * vec4(position, 1.0);  
	// gl_Position = projection * view * model * vec4(position, 1.0);  
	vertexTexCoord = position;
}  