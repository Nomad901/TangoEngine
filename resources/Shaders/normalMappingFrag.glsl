#version 430 core

out vec4 fragColor;

in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 tangentLightPos;
in vec3 tangentViewPos;
in vec3 tangentFragPos;
in vec3 lightPos;
in vec3 viewPos;

uniform sampler2D uDiffuseMap;
uniform sampler2D uNormalMap;
uniform sampler2D uDepthMap;

uniform float uHeightScale;

vec2 parallaxMapping(vec2 pTexCoords, vec3 pViewDir)
{
	const float MIN_LAYERS = 8.0f;
	const float MAX_LAYERS = 32.0f;
	float numLayers = mix(MAX_LAYERS, MIN_LAYERS, abs(dot(vec3(0.0f, 0.0f, 1.0f), pViewDir)));
	float layerDepth = 1.0f / numLayers;
	float currentLayerDepth = 0.0f;

	vec2 p = pViewDir.xy * uHeightScale;
	vec2 deltaTexCoords = p / numLayers;
	
	vec2 currentTexCoords = pTexCoords;
	float currentDepthValue = texture(uDepthMap, currentTexCoords).r;

	while (currentLayerDepth < currentDepthValue)
	{
		currentTexCoords -= deltaTexCoords;
		currentDepthValue = texture(uDepthMap, currentTexCoords).r;
		currentLayerDepth += layerDepth;
	}

	return currentTexCoords;
}

void main() 
{
	vec3 viewDir = normalize(fragPos - viewPos);
	vec2 parallaxTexCoord = parallaxMapping(fragTexCoord, viewDir);
	if (parallaxTexCoord.x > 1.0f || parallaxTexCoord.y > 1.0f ||
		parallaxTexCoord.x < 0.0f || parallaxTexCoord.y < 0.0f)
	{
		discard;
	}
	vec3 normal = texture(uNormalMap, parallaxTexCoord).rgb;
	normal = normalize(normal * 2.0f - 1.0f);
	
	vec3 color = texture(uDiffuseMap, parallaxTexCoord).rgb;
	vec3 ambient = 0.1f * color;
	
	vec3 lightDir = normalize(fragPos - lightPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * color;

	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);

	vec3 specular = vec3(0.2f) * spec;
	fragColor = vec4(ambient + diffuse + specular, 1.0f);
}