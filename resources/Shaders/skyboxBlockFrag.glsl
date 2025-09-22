#version 440

out vec4 outColor;

in vec3 fragPos;
in vec3 fragNormals;

uniform vec3 uCameraPos;
uniform samplerCube uSkybox;

void main()
{
	float ratio = 1.52f; 
	vec3 I   = normalize(fragPos - uCameraPos);
	vec3 R   = refract(I, normalize(fragNormals), ratio);
	outColor = vec4(texture(uSkybox, R).rgb, 1.0f);
}