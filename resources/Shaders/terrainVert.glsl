#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 posTex;
layout(location = 4) in float lightFactor;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normals;

uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uModel;

void main()
{
	vec4 worldPos = uModel * vec4(pos, 1.0f);
	TexCoord = posTex;
	FragPos = worldPos.xyz;
	Normals = (uModel * vec4(normals, 0.0f)).xyz;
	gl_Position = uProj * uView * worldPos;
}