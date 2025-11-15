#version 430 core
layout (location = 0) out vec4 fragPosOut;
layout (location = 1) out vec4 diffuseOut;
layout (location = 2) out vec4 normalsOut;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normals;

uniform sampler2D uColorMap;

void main()
{
	fragPosOut = vec4(FragPos, 1.0f);
	diffuseOut = texture(uColorMap, TexCoord).rgba;
	normalsOut = vec4(normalize(Normals), 1.0f);
}