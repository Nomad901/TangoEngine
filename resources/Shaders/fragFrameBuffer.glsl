#version 440

in vec2 fragPosTex;

out vec4 outColor;

uniform sampler2D uTexture;

void main()
{
	vec3 color = texture(uTexture, fragPosTex).rgb;
	outColor = vec4(color, 1.0f);
}