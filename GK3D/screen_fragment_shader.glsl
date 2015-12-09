#version 150 core
in vec2 vertexTexCoords;
#define M_PI 3.1415926535897932384626433832795

out vec4 color;

uniform sampler2D screenTexture;

float blurWeight(int x, int y, float L)
{
	float L22 = (2 * L* L);
	return (1/(M_PI * L22))*exp(-(x*x + y*y)/L22);
}

void main()
{
	int degree = 11;
	vec3 pixColor = texture(screenTexture, vertexTexCoords).rgb;
	vec2 tex_offset = 1.0 / textureSize(screenTexture, 0);
	float L = 0.3*pixColor.r + 0.59*pixColor.g * 0.11*pixColor.b;
	vec3 result = vec3(0, 0, 0);
	if (L > 0.1)
	{
		float blurNorm = 0;
		for(int x = -int(floor(degree/2)); x < int(floor(degree/2)); x++)
		{
			for(int y = -int(floor(degree/2)); y < int(floor(degree/2)); y++)
			{
				blurNorm += blurWeight(x, y, 1/L);
			}
		}

		for(int x = -int(floor(degree/2)); x < int(floor(degree/2)); x++)
		{
			for(int y = -int(floor(degree/2)); y < int(floor(degree/2)); y++)
			{
				vec3 texXYColor = texture(screenTexture, vertexTexCoords + vec2(tex_offset.x * x, tex_offset.y * y)).rgb;
				result += texXYColor * blurWeight(x, y, 1/L) / blurNorm;
			}
		}
	}
	else
	{
		result = pixColor;
	}
    color = vec4(result, 1.0f);
}