#version 430 core
layout (location = 0) out vec4 fragPosOut;
layout (location = 1) out vec4 diffuseOut;
layout (location = 2) out vec4 normalsOut;
layout (location = 3) out vec4 extraComponentsOut;
layout (location = 4) out vec4 colorBufferOut;

//out vec4 fragColor; 

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normals;

uniform sampler2D uColorMap;

uniform float uMetallic;
uniform float uRoughness;
uniform float uEmissive;

void main()
{
	//fragColor = texture(uColorMap, TexCoord);
	fragPosOut = vec4(FragPos, 1.0f);
	diffuseOut = texture(uColorMap, TexCoord).rgba;
	normalsOut = vec4(normalize(Normals), 1.0f);

	extraComponentsOut = vec4(uMetallic, uRoughness, uEmissive, 0.0f);
	
	vec3 diffuseColor = texture(uColorMap, TexCoord).rgb;
	colorBufferOut = vec4(diffuseColor, 0.5f);
}