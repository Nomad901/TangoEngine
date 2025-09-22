#version 440

in vec2 fragPosTex;

out vec4 outColor;

uniform sampler2D uTexture;
uniform float surroundNum;
uniform float insideNum;

const float offset = 1.0f / 300.0f;

void main()
{
	vec2 offsets[9] = vec2[]
	(
		vec2(-offset,  offset),
		vec2(0.0f,     offset),
		vec2(offset,   offset),
		vec2(-offset,    0.0f),
		vec2(0.0f,       0.0f),
		vec2(offset,     0.0f),
		vec2(-offset, -offset),
		vec2(0.0f,    -offset),
		vec2(offset,  -offset)
	);
	
	float kernel[9] = float[]
	(
		surroundNum, surroundNum, surroundNum,
		surroundNum, insideNum, surroundNum,
		surroundNum, surroundNum, surroundNum
	);

	vec3 sampleTex[9];
	for(int i = 0; i < 9; ++i)
	{
		sampleTex[i] = vec3(texture(uTexture, fragPosTex.st + offsets[i]));
	}
	vec3 color = vec3(0.0f);
	for(int i = 0; i < 9; ++i)
	{
		color += sampleTex[i] * kernel[i]; 
	}
	outColor = vec4(color, 1.0f);
}