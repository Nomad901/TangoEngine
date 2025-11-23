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

	reflectionTexCoord += distortion1;
	reflectionTexCoord = clamp(reflectionTexCoord, 0.001f, 0.999f);

	refractionTexCoord += distortion1;
	refractionTexCoord.x = clamp(refractionTexCoord.x,  0.001f, 0.999f);
	refractionTexCoord.y = clamp(refractionTexCoord.y, -0.999f, -0.001f);

	vec4 reflectionTexture = texture(uReflectionTexture, reflectionTexCoord);
	vec4 refractionTexture = texture(uRefractionTexture, refractionTexCoord);
	
	fragColor = mix(reflectionTexture, refractionTexture, 0.5f);
}
