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

uniform float uMoveFactor;

const float waveStrength = 0.02f;
const float shineDamper  = 20.0f;
const float reflection   = 0.6f;

void main()
{
	vec2 ndc = (fragClipSpace.xy / fragClipSpace.w) / 2.0f + 0.5f;
	vec2 reflectionTexCoord = vec2(ndc.x, 1.0 - ndc.y);  
	vec2 refractionTexCoord = vec2(ndc.x, ndc.y);

	float depth = texture(uDepthMap, refractionTexCoord).r;
	float floorDistance = 2.0f * uNearPlane * uFarPlane / (uFarPlane + uNearPlane - (2.0f * depth - 1.0f) * (uFarPlane - uNearPlane));
	
	depth = gl_FragCoord.z;
	float waterDistance = 2.0f * uNearPlane * uFarPlane / (uFarPlane + uNearPlane - (2.0f * depth - 1.0f) * (uFarPlane - uNearPlane));
	float waterDepth = floorDistance - waterDistance;

	vec2 distortionTexCoords = texture(uDuDvMap, vec2(fragTexCoord.x + uMoveFactor, fragTexCoord.y)).rg * 0.1f;
	distortionTexCoords = fragTexCoord + vec2(distortionTexCoords.x, distortionTexCoords.y + uMoveFactor);
	vec2 totalDistortion = (texture(uDuDvMap, distortionTexCoords).rg * 2.0f - 1.0f) * waveStrength * clamp(waterDepth / 20.0f, 0.0f, 1.0f);
	
	refractionTexCoord += totalDistortion;
	reflectionTexCoord += totalDistortion;
	
	refractionTexCoord = clamp(refractionTexCoord, 0.001f, 0.999f);
	reflectionTexCoord = clamp(reflectionTexCoord, 0.001f, 0.999f);  
	
	vec4 refractionTexture = texture(uRefractionTexture, refractionTexCoord);
	vec4 reflectionTexture = texture(uReflectionTexture, reflectionTexCoord);
	
	vec4 normalMap = texture(uNormalMap, distortionTexCoords);
	vec3 normal = vec3(normalMap.r * 2.0f - 1.0f, normalMap.b * 3.0f, normalMap.g * 2.0f - 1.0f);
	normal = normalize(normal);

	vec3 normalizedCamVec = normalize(fragToCameraVector);
	float fresnelEffectFactor = dot(normalizedCamVec, normal);
	//fresnelEffectFactor = pow(fresnelEffectFactor, 2.0f);
	
	vec3 reflectLight = reflect(normalize(fragFromLightVector), normal);
	float specular = max(dot(reflectLight, normalizedCamVec), 0.0f);
	specular = pow(specular, shineDamper);
	vec3 totalLight = uLightColor * specular * reflection * clamp(waterDepth / 5.0f, 0.0f, 1.0f);

	fragColor = mix(reflectionTexture, refractionTexture, fresnelEffectFactor);
	fragColor = mix(fragColor, vec4(0.0f, 0.3f, 0.5f, 1.0f), 0.2f) + vec4(totalLight, 0.0f);
	fragColor.a = clamp(waterDepth / 5.0f, 0.0f, 1.0f);
}