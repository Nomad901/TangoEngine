#version 430 core
layout(location = 0) in vec3 pos;

out vec4 fragClipSpace;
out vec2 fragTexCoord;
out vec3 fragToCameraVector;
out vec3 fragFromLightVector;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform vec3 uCameraPos;
uniform vec3 uLightPos;

const float tiling = 4.0f;

void main()
{
	vec4 worldSpace = uModel * vec4(pos.x, 0.0f, pos.y, 1.0f);
	fragClipSpace = uProj * uView * worldSpace;
	gl_Position = fragClipSpace;
	fragTexCoord = vec2(pos.x / 2.0f + 0.5f, pos.y / 2.0f + 0.5f) * tiling;
	fragToCameraVector = uCameraPos - worldSpace.xyz;

	fragFromLightVector = worldSpace.xyz - uLightPos; 
}