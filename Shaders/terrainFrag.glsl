#version 430 core
layout(location = 0) out vec3 gPos;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in vec4 Color;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normals;

uniform sampler2D uDiffuseTex;
uniform sampler2D uSpecularTex;

void main()
{
	gPos = FragPos;
	gNormal = normalize(Normals);
	gAlbedoSpec.rgb = texture(uDiffuseTex, TexCoord).rgb;
	gAlbedoSpec.a = texture(uSpecularTex, TexCoord).r;
}