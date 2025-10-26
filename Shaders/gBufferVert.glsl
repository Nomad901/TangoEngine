#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 posTex;
layout(location = 4) in float lightFactor;

out vec4 Color;
out vec2 TexCoord;
out vec3 worldPos;
out vec3 Normals;
out float LightFactor;

void main()
{
	
}