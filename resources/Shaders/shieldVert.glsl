#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 posTex;

out vec3 fragPos;
out DATA
{
	vec3 normal;
	vec3 color;
	vec4 realColor;
	vec2 texCoord;
	mat4 proj;
} data_out;

layout (std140, binding = 0) uniform Matrices
{
	mat4 uProj;
	mat4 uViewMatrix;
}; 

uniform mat4 uMVP;
uniform mat4 uModel;
uniform vec4 uRealColor;

void main()
{
	gl_Position = uMVP * vec4(pos, 1.0);

	data_out.realColor = uRealColor;
	data_out.color = vec3(color);
	data_out.texCoord = posTex;
	data_out.normal = mat3(transpose(inverse(uModel))) * normals;
	data_out.proj = uProj;
	fragPos = vec3(uModel * vec4(pos, 1.0f));
}
