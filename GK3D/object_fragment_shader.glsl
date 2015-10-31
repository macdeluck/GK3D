#version 150 core

in vec3 vertexNormal;
in vec3 vertexFragPos; 

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
out vec4 color;

void main()
{    
    // Ambient
    vec3 ambient = lightColor * material.ambient;
  	
    // Diffuse 
    vec3 norm = normalize(vertexNormal);
    vec3 lightDir = normalize(lightPos - vertexFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);
    
    // Specular
    vec3 viewDir = normalize(viewPos - vertexFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f);
}