#version 430 core
layout (location = 0) out vec3 fragPosOut;
layout (location = 1) out vec3 diffuseOut;
layout (location = 2) out vec3 normalsOut;
layout (location = 3) out vec3 texCoordOut;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normals;

uniform sampler2D uColorMap;

void main()
{
	fragPosOut = FragPos;
	diffuseOut = texture(uColorMap, TexCoord).xyz;
	normalsOut = normalize(Normals);
	texCoordOut = vec3(TexCoord, 0.0f);
}