#version 440

in vec3 fragPos;
in vec4 fragColor;
in vec2 fragPosTex;
in vec3 fragNormals;

out vec4 finalColor;

uniform vec3 uColor;

void main()
{
	finalColor = vec4(uColor, 1.0f);
}