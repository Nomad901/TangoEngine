#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 posTex;
layout(location = 4) in float lightFactor;

out vec2 texCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
	gl_Position = uProj * uView * uModel * vec4(pos.x, 0.0f, pos.y, 0.0f);
	texCoord = posTex;
}