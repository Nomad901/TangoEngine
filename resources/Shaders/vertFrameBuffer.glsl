#version 440

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 posTex;

out vec2 fragPosTex;

void main()
{
	gl_Position = vec4(pos.x, pos.y, 0.0f, 1.0f);
	fragPosTex = posTex;
}
