#version 430 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shines;
	sampler2D textures[8];
	int diffuseIndex;
	int specularIndex;
	int isJustColored;
};

struct Light
{
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;

	vec3 color;
	float intensity;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	vec3 minBounds;
	vec3 maxBounds;
	int useBounds;
};

in DATA
{
	vec3 normal;
	vec3 color;
	vec2 texCoord;
	mat4 proj;
} data_in;

in vec3 fragPos;

out vec4 finalColor;

uniform Material material;
uniform Light light[16];
uniform int uNumberOfLights;
uniform vec3 cameraPos;
uniform vec3 lightPos;

uniform vec3 uObjectColor;

float near = 0.1f;
float far = 150.0f;

float linearizeDepth(float pDepth)
{
	float z = pDepth * 2.0f - 1.0f;
	return (2.0f * near * far) / (far + near - z * (far - near));
}

void main()
{
	vec4 finalResult = vec4(0.0f);
	vec4 texColor = vec4(0.0f);
	float alpha = 1.0f;

	if(material.isJustColored != 1)
	{
		texColor = texture(material.textures[material.diffuseIndex], data_in.texCoord);
	    alpha = texColor.a;
	}

	for(int i = 0; i < uNumberOfLights; ++i)
	{
		// ambient
		vec4 ambient = vec4(0.0f);
		if(material.isJustColored == 1)
		{
			ambient = vec4(light[i].color, 1.0f) * vec4(light[i].ambient, 1.0f) * vec4(material.ambient, 1.0f) * light[i].intensity;
		} 
		else 
		{
			ambient = vec4(light[i].color, 1.0f) * vec4(light[i].ambient, 1.0f) * vec4(material.ambient, 1.0f) * 
					  light[i].intensity * texColor;
		}
		
		// difuse 
		vec3 norm = normalize(data_in.normal);
		vec3 lightDir = normalize(light[i].position - fragPos);
		float diff = max(dot(norm, lightDir), 0.0f);
		vec4 diffuse = vec4(0.0f);
		if(material.isJustColored == 1)
		{
			diffuse = vec4(light[i].color, 1.0f) * vec4(light[i].diffuse, 1.0f) * vec4((diff * material.diffuse), 1.0f) * light[i].intensity;
		}
		else
		{
			diffuse = vec4(light[i].color, 1.0f) * vec4(light[i].diffuse, 1.0f) * vec4((diff * material.diffuse), 1.0f) * 
					  light[i].intensity * texColor;
		}

		// specular
		vec3 viewDir = normalize(cameraPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shines);
		vec4 specular = vec4(0.0f);
		if(material.isJustColored == 1)
		{	
			specular = vec4(light[i].color, 1.0f) * vec4(light[i].specular, 1.0f) * vec4((spec * material.specular), 1.0f) * light[i].intensity;
		}
		else
		{
			specular = vec4(light[i].color, 1.0f) * vec4(light[i].specular, 1.0f) * vec4((spec * material.specular), 1.0f) *
					   light[i].intensity * vec4(texture(material.textures[material.specularIndex], data_in.texCoord).rgb, 1.0f);
		}
		
		// attenuation
		//float distance = length(light[i].position - fragPos);
		//float attenuation = 1.0f / (light[i].constant + light[i].linear * distance + light[i].quadratic * (distance * distance));
		//
		//ambient  *= attenuation;
		//diffuse  *= attenuation;
		//specular *= attenuation;
		 
		finalResult += ambient + diffuse + specular;
	}

	vec4 result = vec4(0.0f);
	result = finalResult * vec4(uObjectColor, 1.0f);
	if(material.isJustColored != 1)
	{
		result.a = alpha;
	}
	finalColor = result;
}