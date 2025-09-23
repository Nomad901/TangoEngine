#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 posTex;

out VS_OUT
{
	vec3 normal;
} vs_out;

uniform mat4 uModel;
uniform mat4 uView;

void main()
{
	gl_Position = uView * uModel * vec4(pos, 1.0);
	mat3 normalMatrix = mat3(transpose(inverse(uView * uModel)));
	vs_out.normal = normalize(vec3(vec4(normalMatrix * normals, 0.0f)));
}
