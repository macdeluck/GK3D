#version 150 core

in vec3 vertexNormal;
in vec3 vertexFragPos; 

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

uniform Light light;
uniform SpotLight spotLight;
uniform Material material;
uniform vec3 viewPos;
out vec4 color;

void main()
{    
    vec3 lightDir = normalize(light.position - vertexFragPos);

    // Ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // Diffuse 
    vec3 norm = normalize(vertexNormal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // Specular
    vec3 viewDir = normalize(viewPos - vertexFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

	// Attenuation
	float distance    = length(light.position - vertexFragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + 
    				light.quadratic * (distance * distance));

    vec3 result = (ambient + diffuse + specular)*attenuation;
	

	// SPOTLIGHT
    vec3 spotLightDir = normalize(spotLight.position - vertexFragPos);
	float theta = dot(spotLightDir, normalize(-spotLight.direction));
    
	if(theta > spotLight.cutOff) 
	{
		// Ambient
		ambient = spotLight.ambient * material.ambient;
  	
		// Diffuse 
		norm = normalize(vertexNormal);
		diff = max(dot(norm, spotLightDir), 0.0);
		diffuse = spotLight.diffuse * (diff * material.diffuse);
    
		// Specular
		viewDir = normalize(viewPos - vertexFragPos);
		reflectDir = reflect(-spotLightDir, norm);  
		spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specular = spotLight.specular * (spec * material.specular);

		// Attenuation
		distance    = length(spotLight.position - vertexFragPos);
		attenuation = 1.0f / (spotLight.constant + spotLight.linear * distance + 
    					spotLight.quadratic * (distance * distance));

		result += (ambient + diffuse + specular)*attenuation;
	}

    color = vec4(result, 1.0f);
}