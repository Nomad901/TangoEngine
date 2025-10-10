#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 posTex;

out vec4 Color;
out vec2 TexCoord;
out vec3 worldPos;

uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uModel;
uniform float uMinHeight;
uniform float uMaxHeight;

void main()
{
	gl_Position = uProj * uView * uModel * vec4(pos, 1.0f);
	float deltaHeight = uMaxHeight - uMinHeight;
	float heightRatio = (pos.y - uMinHeight) / deltaHeight;
	float c = heightRatio * 0.8 + 0.2;
	Color = vec4(c, c, c, 1.0f);
	TexCoord = posTex;
	worldPos = pos;
}
