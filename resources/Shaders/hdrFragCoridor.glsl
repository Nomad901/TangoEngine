#version 430 core

out vec4 fragColorOut;

in vec3 fragPos;
in vec3 fragNormals;
in vec2 fragTexCoord;

struct LightStructure
{
	vec3 mPos;
	vec3 mColor;
};

const uint NUMBER_OF_LIGHTS = 16;
uniform LightStructure lightStructures[NUMBER_OF_LIGHTS];
uniform sampler2D uDiffuseTexture;
uniform vec3 uViewPos;

void main()
{
	fragColorOut = texture(uDiffuseTexture, fragTexCoord);
}