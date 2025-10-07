#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 posTex;

out vec4 Color;

uniform mat4 uView;

void main()
{
	gl_Position = uView * vec4(pos, 1.0f);
	Color = vec4(pos.y / 200.0f);
}
