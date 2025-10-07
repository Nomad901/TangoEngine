#version 430 core

out vec4 fragColor;

in vec4 Color;

uniform sampler2D uGrassTex;

void main()
{
	fragColor = Color;
}