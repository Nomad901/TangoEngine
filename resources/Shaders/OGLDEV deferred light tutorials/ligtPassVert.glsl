#version 430 core
layout (location = 0) in vec3 pos;

uniform mat4 uWVP;

void main()
{
	gl_Position = uWVP * vec4(pos, 1.0f);
}