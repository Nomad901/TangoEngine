#version 430 core

out vec4 fragColorOut;

in vec2 fragTexCoord;

uniform sampler2D uTexture;

void main()
{
	vec3 texColor = texture(uTexture, fragTexCoord).rgb;
	fragColorOut = vec4(texColor, 1.0f);
}