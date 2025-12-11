#version 430 core

out vec4 fragColor;

in vec2 fragTexCoord;

uniform sampler2D uTexture2D;

void main() 
{
	fragColor = texture(uTexture2D, fragTexCoord);
}