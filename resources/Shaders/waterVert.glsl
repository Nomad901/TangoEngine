#version 430 core
layout(location = 0) in vec3 pos;

out vec2 fragTexCoord;
out mat4 fragProjection;
out mat4 fragView;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

const float tiling = 1.0f;

void main()
{
	//vec4 worldSpace = uModel * vec4(pos.x, 0.0f, pos.y, 1.0f);
	fragTexCoord = vec2(pos.x / 2.0f + 0.5f, pos.y / 2.0f + 0.5f) * tiling;
	fragProjection = uProj;
	fragView = uView;
	gl_Position = uModel * vec4(pos, 1.0f);
}