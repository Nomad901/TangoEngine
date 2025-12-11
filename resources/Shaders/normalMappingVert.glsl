#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 texCoord;

out vec2 fragTexCoord;

uniform mat4 uMVP;

void main() 
{
	gl_Position = uMVP * vec4(pos, 1.0f);
	
	fragTexCoord = texCoord;
}