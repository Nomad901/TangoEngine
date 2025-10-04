#version 430 core

out vec4 fragColor;

in float height;

void main()
{
	float h = (height + 16.0f)/32.0f;
	fragColor = vec4(h, h, h, 1.0f);
}