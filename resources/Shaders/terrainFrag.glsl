#version 430 core
layout (location = 0) out vec3 fragPosOut;
layout (location = 1) out vec3 diffuseOut;
layout (location = 2) out vec3 normalsOut;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normals;

uniform sampler2D uColorMap;

void main()
{
	fragPosOut = FragPos;
	diffuseOut = texture(uColorMap, TexCoord).xyz;
	//diffuseOut = vec3(1.0f, 0.0f, 0.0f);
	normalsOut = normalize(Normals);
}