#version 440
layout(location = 0) in vec3 pos;

uniform mat4 uModel;
uniform mat4 uLightSpaceMatrix;

void main()
{
	gl_Position = uLightSpaceMatrix * uModel * vec4(pos, 1.0f);
}
