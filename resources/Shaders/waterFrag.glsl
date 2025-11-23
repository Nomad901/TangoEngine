#version 430 core

out vec4 fragColor;

in vec4 fragClipSpace;
in vec2 fragTexCoord;
in vec3 fragToCameraVector;

uniform sampler2D uReflectionTexture;
uniform sampler2D uRefractionTexture;
uniform sampler2D uDuDvMap;
uniform sampler2D uNormalMap;

uniform float uMoveFactor;

const float waveStrength = 0.2f;

void main()
{
	vec2 ndc = (fragClipSpace.xy / fragClipSpace.w) / 2.0f + 0.5f;
	vec2 reflectionTexCoord = vec2(ndc.x, 1.0 - ndc.y);  
	
	vec2 refractionTexCoord = vec2(ndc.x, ndc.y);
	
	vec2 distortionTexCoords = texture(uDuDvMap, vec2(fragTexCoord.x + uMoveFactor, fragTexCoord.y)).rg * 0.1f;
	distortionTexCoords = distortionTexCoords + vec2(fragTexCoord.x, fragTexCoord + uMoveFactor);
	vec2 totalDistortion = (texture(uDuDvMap, distortionTexCoords).rg * 2.0f - 1.0f) * waveStrength;
	
	refractionTexCoord += totalDistortion;
	reflectionTexCoord += totalDistortion;
	
	refractionTexCoord = clamp(refractionTexCoord, 0.001f, 0.999f);
	reflectionTexCoord = clamp(reflectionTexCoord, 0.001f, 0.999f);  
	
	vec4 reflectionTexture = texture(uReflectionTexture, reflectionTexCoord);
	vec4 refractionTexture = texture(uRefractionTexture, refractionTexCoord);
	
	vec3 normalizedCamVec = normalize(fragToCameraVector);
	float fresnelEffectFactor = dot(normalizedCamVec, vec3(0.0f, 1.0f, 0.0f));
	
	vec4 normalMap = texture(uNormalMap, distortionTexCoords);

	fragColor = mix(reflectionTexture, refractionTexture, fresnelEffectFactor);
	fragColor = mix(fragColor, vec4(0.0f, 0.3f, 0.5f, 1.0f), 0.2f);
	fragColor = normalMap;
}
