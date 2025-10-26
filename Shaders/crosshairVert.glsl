#version 440

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 posTex;
layout(location = 2) in vec4 color;

out vec2 fragPos;
out vec2 fragPosTex;
out vec4 fragColor;

void main()
{
	gl_Position = vec4(pos, 0.0f, 1.0f);

	fragColor = color;
	fragPosTex = posTex;
	fragPos = pos;
}
