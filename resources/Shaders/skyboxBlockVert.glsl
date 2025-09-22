#version 440

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 posTex;

out vec3 fragPos;
out vec3 fragNormals;

uniform mat4 uMVP;
uniform mat4 uModel;

void main()
{
	gl_Position = uMVP * vec4(pos, 1.0);

	fragNormals = mat3(transpose(inverse(uModel))) * normals;
	fragPos = vec3(uModel * vec4(pos, 1.0f));
}
