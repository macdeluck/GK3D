#version 150 core

in vec3 vertexNormal;
in vec3 vertexFragPos;
in vec2 vertexTexCoord;

struct Texture {
	int used;
	sampler2D texture;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

	Texture diffuseTex;
	Texture alphaTex;
};

struct PointLight {
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
	float outerCutOff;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

#define MAX_POINT_LIGHTS_NUM 20
#define MAX_SPOT_LIGHTS_NUM 10

#if MAX_POINT_LIGHTS_NUM > 0
uniform PointLight pointLights[MAX_POINT_LIGHTS_NUM];
uniform int pointLightsNum;
#endif
#if MAX_SPOT_LIGHTS_NUM > 0
uniform SpotLight spotLights[MAX_SPOT_LIGHTS_NUM];
uniform int spotLightsNum;
#endif
uniform Material material;
uniform vec3 viewPos;
out vec4 color;

#if MAX_POINT_LIGHTS_NUM > 0
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));
	
    // Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
	if (material.diffuseTex.used != 0)
	{
		diffuse *= vec3(texture(material.diffuseTex.texture, vertexTexCoord));
	}
    vec3 specular = light.specular * (spec * material.specular);
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
#endif

#if MAX_SPOT_LIGHTS_NUM > 0
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - vertexFragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    
    
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));
	
    // Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
	if (material.diffuseTex.used != 0)
	{
		diffuse *= vec3(texture(material.diffuseTex.texture, vertexTexCoord));
	}
    vec3 specular = light.specular * (spec * material.specular);
    ambient  *= attenuation*intensity;
    diffuse  *= attenuation*intensity;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
#endif

void main()
{
    vec3 norm = normalize(vertexNormal);
    vec3 viewDir = normalize(viewPos - vertexFragPos);

	vec3 result = vec3(0.0, 0.0, 0.0);
	
#if MAX_POINT_LIGHTS_NUM > 0
    for(int i = 0; i < pointLightsNum; i++)
        result += CalcPointLight(pointLights[i], norm, vertexFragPos, viewDir);
#endif

#if MAX_SPOT_LIGHTS_NUM > 0
    for(int i = 0; i < spotLightsNum; i++)
        result += CalcSpotLight(spotLights[i], norm, vertexFragPos, viewDir);
#endif
	float transparency = 1.0;
    if (material.alphaTex.used != 0)
		transparency = float(texture(material.alphaTex.texture, vertexTexCoord));
    color = vec4(result, transparency);
}