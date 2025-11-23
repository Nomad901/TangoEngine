#version 430 core
layout(location = 0) in vec3 pos;

out vec4 fragClipSpace;
out vec2 fragTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

const float tiling = 6.0f;

void main()
{
	fragClipSpace = uProj * uView * uModel * vec4(pos.x, 0.0f, pos.y, 1.0f);
	gl_Position = fragClipSpace;
	fragTexCoord = vec2(pos.x / 2.0f + 0.5f, pos.y / 2.0f + 0.5f) * tiling;
}