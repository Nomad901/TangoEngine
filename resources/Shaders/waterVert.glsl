#version 430 core
layout(location = 0) in vec3 pos;

out vec2 texCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
	gl_Position = uProj * uView * uModel * vec4(pos.x, 0.0f, pos.y, 1.0f);
	texCoord = vec2(pos.x / 2.0f + 0.5f, pos.y / 2.0f + 0.5f);
}