#version 440

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 posTex;

out fragOut
{
	vec3 fragPos;
	vec4 fragColor;
	vec2 fragPosTex;
	vec3 fragNormals;
} outFragOut;

uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat4 uProj;
uniform mat4 uViewMatrix;

void main()
{
	gl_Position = uMVP * vec4(pos, 1.0);

	outFragOut.fragColor = color;
	outFragOut.fragPosTex = posTex;
	outFragOut.fragNormals = mat3(transpose(inverse(uModel))) * normals;
	outFragOut.fragPos = vec3(uModel * vec4(pos, 1.0f));
}
