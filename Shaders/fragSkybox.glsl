#version 440

in vec3 fragPos;

out vec4 outColor;

uniform samplerCube uSkybox;

void main()
{
	outColor = texture(uSkybox, fragPos);
}