#version 440
layout(location = 0) in vec3 pos;

out vec3 fragPos;

uniform mat4 uMVP;

void main()
{
	fragPos = pos;
	vec4 tmpPos = uMVP * vec4(pos, 1.0f);
	gl_Position = tmpPos.xyww;
}
