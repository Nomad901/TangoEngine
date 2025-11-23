#version 430 core

out vec4 fragColor;

in vec4 fragClipSpace;
in vec2 fragTexCoord;

uniform sampler2D uReflectionTexture;
uniform sampler2D uRefractionTexture;
uniform sampler2D uDuDvMap;

uniform float uMoveFactor;

const float waveStrength = 0.2f;

void main()
{
	vec2 ndc = (fragClipSpace.xy / fragClipSpace.w) / 2.0f + 0.5f;
	vec2 reflectionTexCoord = vec2(ndc.x, -ndc.y);
	vec2 refractionTexCoord = vec2(ndc.x,  ndc.y);
	
	vec2 distortion1 = (texture(uDuDvMap, vec2(fragTexCoord.x + uMoveFactor, fragTexCoord.y)).rg * 2.0f - 1.0f) * waveStrength;
	vec2 distortion2 = (texture(uDuDvMap, vec2(-fragTexCoord.x + uMoveFactor, fragTexCoord.y + uMoveFactor)).rg * 2.0f - 1.0f) * waveStrength;
	vec2 totalDistortion = distortion1 + distortion2;

	refractionTexCoord += totalDistortion;
	refractionTexCoord = clamp(refractionTexCoord, 0.001f, 0.999f);
	
	reflectionTexCoord += totalDistortion;
	reflectionTexCoord.x = clamp(reflectionTexCoord.x,  0.001f, 0.999f);
	reflectionTexCoord.y = clamp(reflectionTexCoord.y, -0.999f, -0.001f);
	
	vec4 reflectionTexture = texture(uReflectionTexture, reflectionTexCoord);
	vec4 refractionTexture = texture(uRefractionTexture, refractionTexCoord);
	
	fragColor = mix(reflectionTexture, refractionTexture, 0.5f);
	fragColor = mix(fragColor, vec4(0.0f, 0.3f, 0.5f, 1.0f), 0.2f);
}
