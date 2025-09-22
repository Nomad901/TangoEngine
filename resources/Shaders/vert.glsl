#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 posTex;

out VS_OUT
{
	vec3 fragPos;
	vec4 fragColor;
	vec2 fragPosTex;
	vec3 fragNormals;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
	mat4 uProj;
	mat4 uViewMatrix;
}; 

uniform mat4 uMVP;
uniform mat4 uModel;

void main()
{
	gl_Position = uMVP * vec4(pos, 1.0);
	
	vs_out.fragColor = color;
	vs_out.fragPosTex = posTex;
	vs_out.fragNormals = mat3(transpose(inverse(uModel))) * normals;
	vs_out.fragPos = vec3(uModel * vec4(pos, 1.0f));
}
