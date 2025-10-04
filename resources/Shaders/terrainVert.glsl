#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 posTex;

out vec2 texCoord;

uniform mat4 uMVP;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	gl_Position = uMVP * vec4(pos, 1.0f);
	texCoord = posTex;
}
