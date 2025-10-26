#version 430 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D uText;
uniform vec3 uTextColor;

void main()
{
	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(uText, texCoord).r);
	fragColor = vec4(uTextColor, 1.0f) * sampled;
}