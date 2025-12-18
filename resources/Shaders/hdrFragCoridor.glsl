#version 430 core

out vec4 fragColorOut;

in vec3 fragPos;
in vec3 fragNormals;
in vec2 fragTexCoord;

struct LightStructure
{
	vec3 mPos;
	vec3 mColor;
};

const uint NUMBER_OF_LIGHTS = 16;
uniform LightStructure lightStructures[NUMBER_OF_LIGHTS];
uniform int uNumberLightsToProcces;
uniform sampler2D uDiffuseTexture;

void main()
{
	vec3 color = texture(uDiffuseTexture, fragTexCoord).rgb;
	vec3 normalizedNormals = normalize(fragNormals);

	vec3 lightingResult = vec3(0.0f);
	for (int i = 0; i < uNumberLightsToProcces; ++i)
	{
		vec3 lightDir = normalize(lightStructures[i].mPos - fragPos);
		float diff = max(dot(lightDir, normalizedNormals), 0.0f);
		vec3 diffuse = lightStructures[i].mColor * diff * color;
		vec3 result = diffuse;

		float distance = length(fragPos - lightStructures[i].mPos);
		float attenuation = 1.0f / (1.0f + 0.09f * distance + 0.032f * distance * distance);
		result *= attenuation;
		lightingResult += result;	
	}

	fragColorOut = vec4(lightingResult, 1.0f);
}