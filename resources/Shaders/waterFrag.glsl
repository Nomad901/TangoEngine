#version 430 core

out vec4 fragColor;

in vec4 fragClipSpace;
in vec2 fragTexCoord;
in vec3 fragToCameraVector;
in vec3 fragFromLightVector;

uniform sampler2D uReflectionTexture;
uniform sampler2D uRefractionTexture;
uniform sampler2D uDuDvMap;
uniform sampler2D uNormalMap;
uniform sampler2D uDepthMap;
uniform vec3 uLightColor;
uniform float uNearPlane;
uniform float uFarPlane;
uniform vec4 uWaterColor;
uniform float uWaterRoughness;
uniform float uWaterMetallic;

uniform float uMoveFactor;

const float waveStrength = 0.02f;
const float shineDamper  = 20.0f;
const float reflection   = 0.6f;

float getWaterDepth(vec2 pRefractionTexCoord)
{
	float depthTexture = texture(uDepthMap, pRefractionTexCoord).r;
	float floorDistance = 2.0f * uNearPlane * uFarPlane / (uFarPlane + uNearPlane - (2.0f * depthTexture - 1.0f) * (uFarPlane - uNearPlane));
	
	float depthWater = gl_FragCoord.z;
	float waterDistance = 2.0f * uNearPlane * uFarPlane / (uFarPlane + uNearPlane - (2.0f * depthWater - 1.0f) * (uFarPlane - uNearPlane));
	float waterDepth = floorDistance - waterDistance;

	return waterDepth;
}

vec2 getDistortionTexCoords()
{
	vec2 distortionTexCoords = texture(uDuDvMap, vec2(fragTexCoord.x + uMoveFactor, fragTexCoord.y)).rg * 0.1f;
	distortionTexCoords = fragTexCoord + vec2(distortionTexCoords.x, distortionTexCoords.y + uMoveFactor);

	return distortionTexCoords;
}

vec2 getTotalDistortion(float pWaterDepth, vec2 pDistortionTexCoords)
{
	vec2 totalDistortion = (texture(uDuDvMap, pDistortionTexCoords).rg * 2.0f - 1.0f) * waveStrength * clamp(pWaterDepth / 20.0f, 0.0f, 1.0f);

	return totalDistortion;
}

vec3 getNormal(vec2 pDistortionTexCoords)
{
	vec4 normalMap = texture(uNormalMap, pDistortionTexCoords);
	vec3 normal = vec3(normalMap.r * 2.0f - 1.0f, normalMap.b * 3.0f, normalMap.g * 2.0f - 1.0f);
	//vec3 normal = normalize(normalMap.rgb * 2.0f - 1.0f);
	normal = normalize(normal);

	return normal;
}

float getFresnelEffectFactor(vec3 pNormal, float pMultiplier)
{
	vec3 normalizedCamVec = normalize(fragToCameraVector);
	float fresnelEffectFactor = dot(normalizedCamVec, pNormal);

	fresnelEffectFactor = pow(fresnelEffectFactor, pMultiplier);
	
	return fresnelEffectFactor;
}

vec3 calculateLight(vec3 pNormal, float pWaterDepth)
{
	vec3 reflectLight = reflect(normalize(fragFromLightVector), pNormal);
	vec3 normalizedCamVec = normalize(fragToCameraVector);
	float specular = max(dot(reflectLight, normalizedCamVec), 0.0f);
	specular = pow(specular, shineDamper);
	vec3 totalLight = uLightColor * specular * reflection * clamp(pWaterDepth / 5.0f, 0.0f, 1.0f);
	
	return totalLight;
}

void main()
{
	vec2 ndc = (fragClipSpace.xy / fragClipSpace.w) / 2.0f + 0.5f;
	vec2 reflectionTexCoord = vec2(ndc.x, 1.0 - ndc.y);  
	vec2 refractionTexCoord = vec2(ndc.x, ndc.y);

	float waterDepth = getWaterDepth(refractionTexCoord);
	
	vec2 distortionTexCoords = getDistortionTexCoords();
	vec2 totalDistortion = getTotalDistortion(waterDepth, distortionTexCoords);
	refractionTexCoord += totalDistortion;
	reflectionTexCoord += totalDistortion;
	refractionTexCoord = clamp(refractionTexCoord, 0.001f, 0.999f);
	reflectionTexCoord = clamp(reflectionTexCoord, 0.001f, 0.999f);  
	
	vec4 refractionTexture = texture(uRefractionTexture, refractionTexCoord);
	vec4 reflectionTexture = texture(uReflectionTexture, reflectionTexCoord);
	
	vec3 normal = getNormal(distortionTexCoords);

	float fresnelEffectFactor = getFresnelEffectFactor(normal, 1.0f);

	vec3 light = calculateLight(normal, waterDepth);
	
	fragColor = mix(reflectionTexture, refractionTexture, fresnelEffectFactor);
	fragColor = mix(fragColor, uWaterColor, 0.2f) + vec4(light, 0.0f);
	fragColor.a = clamp(waterDepth / 5.0f, 0.0f, 1.0f);
}