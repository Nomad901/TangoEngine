#version 430 core

in DATA
{
	vec3 normal;
	vec3 color;
	vec4 realColor;
	vec2 texCoord;
	mat4 proj;
} data_in;
in vec3 fragPos;

out vec4 finalColor;

void main()
{
	finalColor = data_in.realColor;
}